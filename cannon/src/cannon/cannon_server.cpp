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
#include <glog/logging.h>

#include "interface/gen-cpp/Cannon.h"
#include "kv_schema.h"
#include "storage.h"

#include "version.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

namespace paradigm4 {
namespace cannon {

class CannonHandler : public CannonIf {
public:
    CannonHandler(Storage *s) {
        storage = s;
    }

    void get(CannonResponse& res, const CannonRequest& req) {
        boost::timer t;
        res.values.reserve(req.keys.size());
        res.valueStatuses.reserve(req.keys.size());

        res.status = Status::OK;
        for (std::size_t pos = 0; pos < req.keys.size(); pos += storage->get_key_size()) {
            const byte *value = storage->seek((const byte *)(req.keys.data() + pos));
            if (value != nullptr) {
                res.valueStatuses.push_back(ItemStatus::FOUND);
                res.values.append((char *)value, storage->get_value_size());
            } else {
                res.valueStatuses.push_back(ItemStatus::NOT_FOUND);
                // Use sequence of '\0' as the placeholder for "NOT_FOUND" key
                const char placeholder = '\0';
                res.values.append(storage->get_value_size(), placeholder);
                res.status = Status::PARTIAL_OK;
            }
        }

        LOG(INFO) << "[query]key count:" << req.keys.size() / storage->get_key_size() << " res status:" << res.status \
            << " time:" << t.elapsed() * 1000 << "ms";
    }

private:
    const Storage *storage;
};

} // namespace cannon
} // namespace paradigm4

// define gflags
static bool ValidateString(const char* flagname, const std::string &val) {
    return !val.empty();
}

DEFINE_string(kv_schema_path, "", "kv schema path to load");
DEFINE_validator(kv_schema_path, &ValidateString);
DEFINE_string(kv_path, "", "kv path to load");
DEFINE_validator(kv_path, &ValidateString);
DEFINE_int32(port, 9090, "port to listen");

int main(int argc, char* argv[]) {
    google::SetVersionString(std::string("\n") + version);
    google::InitGoogleLogging(argv[0]);
    google::ParseCommandLineFlags(&argc, &argv, true);

    using namespace paradigm4::cannon;

    LOG(INFO) << "loading file:" << FLAGS_kv_schema_path;
    KVSchema kv_schema(FLAGS_kv_schema_path);

    Storage storage(kv_schema);

    boost::timer t;
    bool ret = storage.load(FLAGS_kv_path);
    if (!ret) {
        LOG(ERROR) << "failed to load kv:" << FLAGS_kv_path;
        exit(-1);
    }

    LOG(INFO) << "load time(s):" << t.elapsed();

    TThreadedServer server(
        boost::make_shared<CannonProcessor>(boost::make_shared<CannonHandler>(&storage)),
        boost::make_shared<TServerSocket>(FLAGS_port),
        boost::make_shared<TFramedTransportFactory>(),
        boost::make_shared<TBinaryProtocolFactory>());

    LOG(INFO) << "starting the server on port:" << FLAGS_port;
    server.serve();
    LOG(INFO) << "shut the server down...";
    return 0;
}
