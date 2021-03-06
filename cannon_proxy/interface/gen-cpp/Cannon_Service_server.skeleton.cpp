// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "Cannon_Service.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::paradigm4::cannon_proxy;

class Cannon_ServiceHandler : virtual public Cannon_ServiceIf {
 public:
  Cannon_ServiceHandler() {
    // Your initialization goes here
  }

  void version(std::string& _return) {
    // Your implementation goes here
    printf("version\n");
  }

  bool reload(const std::vector<std::string> & paths) {
    // Your implementation goes here
    printf("reload\n");
  }

  void get(CannonResponse& _return, const CannonRequest& req) {
    // Your implementation goes here
    printf("get\n");
  }

};

int main(int argc, char **argv) {
  int port = 9090;
  shared_ptr<Cannon_ServiceHandler> handler(new Cannon_ServiceHandler());
  shared_ptr<TProcessor> processor(new Cannon_ServiceProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

