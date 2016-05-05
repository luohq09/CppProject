//
// Created by LuoHuaqing on 4/13/16.
//

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "cannon_client_wrapper.h"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

namespace paradigm4 {
namespace cannon_proxy {

void CannonClientWrapper::_init_client() {
    boost::shared_ptr<TTransport> socket(new TSocket(_host, _port));
    boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

    _client = std::make_shared<cannon::CannonClient>(protocol);
}

CannonClientWrapper::CannonClientWrapper(const std::string &host, int port) :
        _host(host), _port(port)
{
    _init_client();
}

CannonClientWrapper::CannonClientWrapper(std::string &&host, int port) :
        _host(std::move(host)), _port(port)
{
    _init_client();
}

CannonClientWrapper::~CannonClientWrapper() {
    auto protocol = _client->getInputProtocol();
    auto transport = protocol->getTransport();
    if (transport->isOpen()) {
        transport->close();
    }
}
/*
std::shared_ptr<ThreadPool> &CannonClientWrapper::_get_insert_thread_pool() {
    if (_insert_thread_pool == nullptr) {
        _insert_thread_pool = std::make_shared<ThreadPool>(1);
    }

    return _insert_thread_pool;
}
*/
bool CannonClientWrapper::operator==(const CannonClientWrapper &other) const
{
    return (_host == other._host && _port == other._port);
}

CannonClientWrapper::insert_ret_type insert_kvs_f(
        std::shared_ptr<cannon::CannonClient> client,
        const CannonClientWrapper::kvs_pointer kvs,
        const int data_version) {
    auto protocol = client->getInputProtocol();
    auto transport = protocol->getTransport();
    if (!transport->isOpen()) {
        transport->open();
    }
    auto ret = client->insert(*kvs, data_version);
    // transport->close();
    return ret;
}

std::future<CannonClientWrapper::insert_ret_type> CannonClientWrapper::enqueue_insert_kvs(const kvs_pointer kvs, int data_version) {
    return _query_thread_pool.enqueue(insert_kvs_f, _client, kvs, data_version);
    // _insert_results.push_back(std::move(result));
}

CannonClientWrapper::query_ret_type query_keys_f(
        std::shared_ptr<cannon::CannonClient> client,
        const CannonClientWrapper::keys_pointer keys,
        const int data_version) {
    auto protocol = client->getInputProtocol();
    auto transport = protocol->getTransport();
    if (!transport->isOpen()) {
        transport->open();
    }
    cannon::CannonResponse rps;
    client->get(rps, *keys, data_version);
    // transport->close();
    CannonClientWrapper::query_ret_type ret(std::move(rps.values));
    return ret;
}

std::future<CannonClientWrapper::query_ret_type> CannonClientWrapper::enqueue_query_keys(const keys_pointer keys, int data_version) {
     return _insert_thread_pool.enqueue(query_keys_f, _client, keys, data_version);
    //_query_results.push_back(std::move(result));
}

bool clear_data_f(std::shared_ptr<cannon::CannonClient> client, const int data_version) {
    auto protocol = client->getInputProtocol();
    auto transport = protocol->getTransport();
    if (!transport->isOpen()) {
        transport->open();
    }
    auto ret = client->clear_data(data_version);
    // transport->close();
    return ret;
}

std::future<bool> CannonClientWrapper::clear_data(int data_version) {
    return _query_thread_pool.enqueue(clear_data_f, _client, data_version);
}
/*
CannonClientWrapper::insert_ret_type CannonClientWrapper::get_insert_results() {
    CannonClientWrapper::insert_ret_type ret = true;
    for (auto &result : _insert_results) {
        auto result_ret = result.get();
        ret &= result_ret;
        // ToDo: log the failed case (i.e. result_ret==false)
    }

    _insert_results.clear();
    _insert_thread_pool = nullptr;
    return ret;
}

CannonClientWrapper::query_ret_type CannonClientWrapper::get_query_results() {
    CannonClientWrapper::query_ret_type ret;
    for (auto &result : _query_results) {
        auto result_ret = result.get();
        ret.insert(ret.end(), result_ret.begin(), result_ret.end());
    }

    _query_results.clear();
    return ret;
}
*/
} // namespace cannon_proxy
} // namespace paradigm4
