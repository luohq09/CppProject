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

#include <boost/make_shared.hpp>
#include <boost/timer.hpp>
#include <iostream>
#include <glog/logging.h>
#include <gflags/gflags.h>

#include "interface/gen-cpp/Cannon.h"
#include "storage.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

namespace paradigm4 {
namespace cannon {

static Storage s_storage;

class CannonHandler : public CannonIf {
public:
    CannonHandler() {}

    void get(CannonResponse& res, const std::vector<int64_t> & keys, const int32_t data_version) {
        boost::timer t;
        for (size_t i = 0; i < keys.size(); ++i) {
            ValueItem val;
            Storage::Status ret = s_storage.seek(keys[i], &val.value, data_version);
            if (ret == Storage::FOUND) {
                DLOG(INFO) << "key:" << keys[i] << " found, value:" << val.value;
                val.status = ItemStatus::FOUND;
            } else {
                DLOG(INFO) << "key:" << keys[i] << " not found.";
                val.status = ItemStatus::NOT_FOUND;
            }

            res.values.push_back(val);
        }
        res.status = Status::OK;
        LOG(INFO) << "[query]key size:" << keys.size() << " res status:" << res.status \
            << " time:" << t.elapsed() * 1000;
    }

    bool insert(const std::vector<CannonKV> & kvs, const int32_t data_version) {
        bool ret = true;
        for (auto &kv : kvs) {
            if (!s_storage.insert(kv.key, kv.value, data_version)) {
                ret = false;
            }
        }
        return ret;
    }

    bool clear_data(const int32_t data_version) {
        s_storage.clear_data(data_version);
        return true;
    }
};

/*
  CannonIfFactory is code generated.
  CannonCloneFactory is useful for getting access to the server side of the
  transport.  It is also useful for making per-connection state.  Without this
  CloneFactory, all connections will end up sharing the same handler instance.
*/
class CannonCloneFactory : virtual public CannonIfFactory {
public:
    virtual ~CannonCloneFactory() {}
    virtual CannonIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) {
        boost::shared_ptr<TSocket> sock = boost::dynamic_pointer_cast<TSocket>(connInfo.transport);
        LOG(INFO) << "peer address: " << sock->getPeerAddress();
        LOG(INFO) << "peer port: "    << sock->getPeerPort();
        return new CannonHandler;
    }

    virtual void releaseHandler(::paradigm4::cannon::CannonIf* handler) {
        delete handler;
    }
};

} // namespace cannon
} // namespace paradigm4

// define gflags
// DEFINE_string(model_path, "", "model path to load");
DEFINE_int32(port, 9090, "port to listen");

int main(int argc, char* argv[]) {
    google::InitGoogleLogging(argv[0]);  
    google::ParseCommandLineFlags(&argc, &argv, true);

    using namespace paradigm4::cannon;

    s_storage.init();
/*
    boost::timer t;
    bool ret = s_storage.load(FLAGS_model_path);
    if (!ret) {
        LOG(ERROR) << "failed to load model:" << FLAGS_model_path;
        exit(-1);
    }

    LOG(INFO) << "load time(s):" << t.elapsed();
*/
    TThreadedServer server(
        boost::make_shared<CannonProcessorFactory>(boost::make_shared<CannonCloneFactory>()),
        boost::make_shared<TServerSocket>(FLAGS_port),
        boost::make_shared<TBufferedTransportFactory>(),
        boost::make_shared<TBinaryProtocolFactory>());

    LOG(INFO) << "starting the server on port:" << FLAGS_port;
    std::cout << "starting the server on port:" << FLAGS_port <<std::endl;
    server.serve();
    LOG(INFO) << "shut the server down...";
    std::cout << "shut the server down..." <<std::endl;
    return 0;
}
