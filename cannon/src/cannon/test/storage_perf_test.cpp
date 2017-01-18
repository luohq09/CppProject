//
// Created by LuoHuaqing on 06/12/2016.
//
#include <boost/timer.hpp>
#include <glog/logging.h>
#include <boost/filesystem.hpp>

#include "storage.h"

namespace paradigm4 {
namespace cannon {
namespace test {
namespace fs = boost::filesystem;

class StoragePerfTest {
public:
    StoragePerfTest(const std::string &kv_schema_path,
                    const std::string &kv_path,
                    std::size_t total_key_count
    ) : _kv_path(kv_path),
        in_key_count(total_key_count / 2),
        random_key_count(total_key_count / 2) {
        KVSchema kv_schema(kv_schema_path);
        _storage = new Storage(kv_schema);
    }

    ~StoragePerfTest() {
        delete _storage;
    }

    void test() {
        storage_load();
        storage_seek();
    }
private:
    Storage *_storage;
    std::string _kv_path;
    const std::size_t in_key_count;
    const std::size_t random_key_count;

private:
    void storage_load() throw() {
        boost::timer timer;

        if(!_storage->load(_kv_path)) {
            std::string errMsg = "Failed to load kv: " + _kv_path;
            throw std::runtime_error(errMsg);
        }

        LOG(INFO) << "Loaded " << _storage->size() << " keys." << " The elapsed time: " << timer.elapsed() << "s";
    }

    void storage_seek() {
        std::vector<byte> in_keys = get_in_keys();
        std::vector<byte> random_keys = get_random_keys();

        boost::timer timer;
        for (std::size_t i = 0; i < in_keys.size(); i += _storage->get_key_size()) {
            _storage->seek(&in_keys[i]);
        }
        LOG(INFO) << "Sought " << in_keys.size() / _storage->get_key_size()
                  << " in_keys." << " The elapsed time: " << timer.elapsed() << "s";

        timer.restart();
        for (std::size_t i = 0; i < random_keys.size(); i += _storage->get_key_size()) {
            _storage->seek(&random_keys[i]);
        }
        LOG(INFO) << "Sought " << random_keys.size() / _storage->get_key_size()
                  << " random_keys." << " The elapsed time: " << timer.elapsed() << "s";
    }


    std::vector<byte> get_in_keys() throw() {
        std::vector<byte> keys;
        keys.reserve(in_key_count * _storage->get_key_size());

        fs::path path(_kv_path);
        if (!fs::exists(path)) {
            std::string errMsg = "path not exists: " + path.string();
            throw std::runtime_error(errMsg);
        }

        if (!fs::is_directory(path)) {
            std::string errMsg = "path not a directory: " + path.string();
            throw std::runtime_error(errMsg);
        }

        fs::directory_iterator end_iter;
        for (fs::directory_iterator dir_itr(path); dir_itr != end_iter; ++dir_itr) {
            auto file_path = dir_itr->path();
            std::ifstream in_stream;
            in_stream.open(file_path.string());
            if (!in_stream.is_open()) {
                std::string errMsg = "can't open file:" + file_path.string();
                in_stream.close();
                throw std::runtime_error(errMsg);
            }

            const std::size_t buf_size = _storage->get_key_size() + _storage->get_value_size();
            byte buf[buf_size];
            std::size_t count = 0;
            while (in_stream.read((char *)buf, buf_size)) {
                keys.insert(keys.end(), buf, buf + _storage->get_key_size());
                ++count;
                if (count >= in_key_count) return keys;
            }
        }

        return keys;
    }

    std::vector<byte> get_random_keys() {
        std::vector<byte> keys;
        keys.reserve(random_key_count * _storage->get_key_size());

        const std::size_t array_len = _storage->get_key_size() / sizeof(int);
        int key[array_len];
        srand(time(nullptr));
        for (std::size_t i = 0; i < random_key_count; ++i) {
            for (std::size_t j = 0; j < array_len; ++j) {
                key[j] = rand();
            }
            byte *p = (byte *)key;
            keys.insert(keys.end(), p, p + _storage->get_key_size());
        }

        return keys;
    }
};

}
}
}

// define gflags
static bool ValidateString(const char *flagname, const std::string &val) {
    return !val.empty();
}

DEFINE_string(kv_schema_path, "", "kv schema path to load");
DEFINE_validator(kv_schema_path, &ValidateString);
DEFINE_string(kv_path, "", "kv path to load");
DEFINE_validator(kv_path, &ValidateString);
DEFINE_uint64(key_count, 10000, "the count of keys used for test");

int main(int argc, char* argv[]) {
    google::InitGoogleLogging(argv[0]);
    google::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_log_dir= "../perf_logs";

    try {
        paradigm4::cannon::test::StoragePerfTest storagePerfTest(FLAGS_kv_schema_path, FLAGS_kv_path, FLAGS_key_count);
        storagePerfTest.test();
    } catch (std::exception &e) {
        LOG(ERROR) << e.what();
    }
}