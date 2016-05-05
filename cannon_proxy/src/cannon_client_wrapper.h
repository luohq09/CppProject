//
// Created by LuoHuaqing on 4/13/16.
//

#ifndef CANNON_PROXY_CANNON_CLIENT_H
#define CANNON_PROXY_CANNON_CLIENT_H

#include <string>
#include <vector>
#include <memory>

#include "Cannon.h"
#include "ThreadPool.h"


namespace paradigm4 {
namespace cannon_proxy {

class CannonClientWrapper {
public:
    typedef bool insert_ret_type;
    typedef std::vector<cannon::ValueItem> query_ret_type;
    typedef std::shared_ptr<std::vector<cannon::CannonKV>> kvs_pointer;
    typedef std::shared_ptr<std::vector<int64_t>> keys_pointer;

    CannonClientWrapper(const std::string &host, int port);
    CannonClientWrapper(std::string &&host, int port);

    CannonClientWrapper(const CannonClientWrapper &) = delete;
    CannonClientWrapper(CannonClientWrapper &&) = delete;
    CannonClientWrapper &operator=(const CannonClientWrapper &) = delete;
    CannonClientWrapper &operator=(CannonClientWrapper &&) = delete;

    // ToDo: virtual (if you want ot inherit it)
    ~CannonClientWrapper();

    bool operator==(const CannonClientWrapper &other) const;
    std::future<insert_ret_type> enqueue_insert_kvs(const kvs_pointer kvs, int data_version);
    std::future<query_ret_type> enqueue_query_keys(const keys_pointer keys, int data_version);
    std::future<bool> clear_data(int data_version);
/*
    insert_ret_type get_insert_results();
    query_ret_type get_query_results();
*/
private:
    void _init_client();
    std::shared_ptr<ThreadPool> &_get_insert_thread_pool();

private:
    friend struct CannonClientWrapper_Hasher;

    std::string _host;
    int _port;

    ThreadPool _query_thread_pool{1}; // 1 means single thread, which is critical since the "cannon" service is not thread-safe
    ThreadPool _insert_thread_pool{1};
    std::shared_ptr<cannon::CannonClient> _client {nullptr};
/*
    std::vector<std::future<insert_ret_type>> _insert_results;
    std::vector<std::future<query_ret_type>> _query_results;
*/
};

struct CannonClientWrapper_Hasher {
    size_t operator()(const CannonClientWrapper &obj) {
        return std::hash<std::string>{}(obj._host + std::to_string(obj._port));
    }
};

} // namespace cannon_proxy
} // namespace paradigm4

#endif //CANNON_PROXY_CANNON_CLIENT_H
