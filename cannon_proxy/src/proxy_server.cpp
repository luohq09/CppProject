//
// Created by LuoHuaqing on 4/12/16.
//
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/TToString.h>

#include <unordered_map>
#include <sstream>
#include <chrono>

#include "../interface/gen-cpp/Cannon_Service.h"

#include "./cannon_client_wrapper.h"
#include "model_files_reader.h"

#include "proxy_server.h"

using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

namespace paradigm4 {
namespace cannon_proxy {

class CannonServiceHandler : public Cannon_ServiceIf {
public:
    typedef int64_t keyT;
    typedef double valueT;

    CannonServiceHandler(const std::vector<std::pair<std::string, int>> &servers) {
        _init_clients(servers);
    }

    ~CannonServiceHandler() {
        delete []_clients;
    }

    void version(std::string& _return) {
        _return = _version;
    }

    void get(CannonResponse& _return, const CannonRequest& req) {
        std::vector<CannonClientWrapper::keys_pointer> sharding_keys(_clients_num);
        for (auto &ptr : sharding_keys) {
            ptr = std::make_shared<CannonClientWrapper::keys_pointer::element_type>();
        }
        int n = 0;
        std::unordered_map<keyT, int> key_idx;

        // Shard the keys
        for (auto key : req.keys) {
            key_idx[key] = n++;
            int client_idx = (int)std::hash<keyT>{}(key) % _clients_num;
            sharding_keys[client_idx]->push_back(key);
        }

        // Enqueue the query tasks
        int data_version = _data_version;
        std::vector<std::future<CannonClientWrapper::query_ret_type>> results;
        for (int i = 0; i < _clients_num; ++i) {
            if (!sharding_keys[i]->empty()) {
                results.push_back(_clients[i].enqueue_query_keys(sharding_keys[i], data_version));
            }
        }

        // Get the results
        _return.values.resize(n);
        Status::type status = Status::OK;
        auto result_it = results.begin();
        auto keys_it = sharding_keys.begin();
        while (keys_it != sharding_keys.end()) {
            const auto &keys = *(keys_it->get());
            if (!keys.empty()) {
                auto result = result_it->get();
                for (size_t j = 0; j < keys.size(); ++j) {
                    auto &item = _return.values[key_idx[keys[j]]];
                    item.value = result[j].value;
                    item.status = (ItemStatus::type)result[j].status;
                    if (item.status == ItemStatus::ERROR) {
                        status = Status::ERROR;
                    } else if (item.status == ItemStatus::NOT_FOUND) {
                        if (status == Status::OK) {
                            status = Status::PARTIAL_OK;
                        }
                    }
                }

                ++result_it;
            }

            ++keys_it;
        }

        _return.status = status;
    }

    bool reload(const std::vector<std::string> &paths) {
        if (_is_loading.test_and_set()) return false;
        if (paths.empty()) return true;

        bool ret = true;
        ModelFilesReader reader(paths);
        const int block_size = 1000;
        std::vector<cannon::CannonKV> kvs;
        while (true) {
            auto lines = reader.get_lines(block_size);
            if (lines.empty()) break;

            kvs.clear();
            for (const auto &line : lines) {
                std::istringstream iss(line);
                keyT key;
                valueT value;
                iss >> key >> value;
                if (!iss.bad()) {
                    kvs.push_back(cannon::CannonKV());
                    kvs.back().key = key;
                    kvs.back().value = value;
                } // ToDo: log the else case
            }
            if (!kvs.empty()) {
                ret &= _insert(kvs);
            }
        }

        // ToDo:
        // "clear_data" is not thread safe and may lead to in-consistent "get" data since the thrift server is multi-threading.
        // However, I don't want to involve "synchronization" which would hurt the performance of the "get" operation.
        // The good thing is: the in-consistence can only be caused by the "enqueue_query_keys" that are still being called
        // with old "data_version" after "clear_data()", which might not be a big deal (not sure). So I just leave it this way.
        // A workaroud is: keep the old "data_version" data around until the next "reload", this workaround can promise the consistence
        // while the duration between two "reload" is long enough, which should be the common scenario in practice.
        if (ret) {
            int data_version = _data_version;
            ++_data_version;

            // Another workaround: sleep for a while to wait for the completion of all the old "enqueue_query_keys"
            std::this_thread::sleep_for(std::chrono::seconds(5));
            // clear data
            std::vector<std::future<bool>> results;
            for (int i = 0; i < _clients_num; ++i) {
                results.push_back(_clients[i].clear_data(data_version));
            }

            for (auto &result : results) {
                ret &= result.get();
            }
        }

        _is_loading.clear();
        return ret;
    }

private:
    void _init_clients(const std::vector<std::pair<std::string, int>> &servers) {
        _clients_num = (int)servers.size();

        if (_clients_num <= 0) {
            _clients = nullptr;
        } else {
            _clients = (CannonClientWrapper *)(new char[_clients_num * sizeof(CannonClientWrapper)]);
            for (int i = 0; i < _clients_num; ++i) {
                // Ugly hacking
                // We can't put the "CannonClientWrapper" obj into a stl
                // container since it is neither copyable nor moveable
                new(_clients + i)CannonClientWrapper(servers[i].first, servers[i].second);
            }
        }
    }

    bool _insert(const std::vector<cannon::CannonKV> &kvs) {
        std::vector<CannonClientWrapper::kvs_pointer> sharding_kvs(_clients_num);
        for (auto &ptr : sharding_kvs) {
            ptr = std::make_shared<CannonClientWrapper::kvs_pointer::element_type>();
        }
        // Shard the keys
        for (const auto &kv : kvs) {
            int client_idx = (int)std::hash<keyT>{}(kv.key) % _clients_num;
            sharding_kvs[client_idx]->push_back(kv);
        }

        // Enqueue the query tasks
        int data_version = _data_version + 1;
        std::vector<std::future<CannonClientWrapper::insert_ret_type>> results;
        for (int i = 0; i < _clients_num; ++i) {
            if (!sharding_kvs[i]->empty()) {
                results.push_back(_clients[i].enqueue_insert_kvs(sharding_kvs[i], data_version));
            }
        }

        // Get the results
        bool ret = true;
        auto result_it = results.begin();
        while (result_it != results.end()) {
            ret &= result_it->get();
            ++result_it;
        }

        return ret;
    }

private:
    CannonClientWrapper *_clients;
    int _clients_num {0};
    std::atomic<int> _data_version {0};
    std::string _version;
    std::atomic_flag _is_loading {ATOMIC_FLAG_INIT};
};

#define bsp boost::shared_ptr
ProxyServer::ProxyServer(int port, const std::vector<std::pair<std::string, int>> &servers) :
        _server(bsp<TProcessor>(new Cannon_ServiceProcessor(bsp<CannonServiceHandler>(new CannonServiceHandler(servers)))),
                bsp<TServerTransport>(new TServerSocket(port)),
                bsp<TTransportFactory>(new TBufferedTransportFactory()),
                bsp<TProtocolFactory>(new TBinaryProtocolFactory()))
{
}
#undef bsp

void ProxyServer::run() {
    _server.serve();
}

ProxyServer::~ProxyServer() {
    _server.stop();
}

} // namespace cannon_proxy
} // namespace paradigm4
