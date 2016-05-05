#include "proxy_server.h"

int main() {
    std::vector<std::pair<std::string, int>> servers;
    servers.push_back(std::make_pair("127.0.0.1", 9991));
    servers.push_back(std::make_pair("127.0.0.1", 9992));
    servers.push_back(std::make_pair("127.0.0.1", 9993));
    paradigm4::cannon_proxy::ProxyServer proxyServer(9998, servers);
    proxyServer.run();
    return 0;
}