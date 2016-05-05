//
// Created by LuoHuaqing on 4/12/16.
//

#ifndef CANNON_PROXY_PROXY_SERVER_H
#define CANNON_PROXY_PROXY_SERVER_H

#include <string>
#include <vector>

#include <thrift/server/TThreadedServer.h>

namespace paradigm4 {
namespace cannon_proxy {

class ProxyServer {
public:
    ProxyServer(int port, const std::vector<std::pair<std::string, int>> &servers);
    ~ProxyServer();
    void run();

private:
    apache::thrift::server::TThreadedServer _server;

};

} // namespace cannon_proxy
} // namespace paradigm4

#endif //CANNON_PROXY_PROXY_SERVER_H
