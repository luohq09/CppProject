//
// Created by LuoHuaqing on 14/12/2016.
//
#include <fstream>
#include <boost/filesystem.hpp>
#include <glog/logging.h>

#include "storage.h"

namespace paradigm4 {
namespace cannon {

static std::ifstream::pos_type get_file_size(const char* filename) {
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

bool Storage::load_kv_file(const fs::path &file_path, std::vector<byte> &kvs) {
    LOG(INFO) << "loading file:" << file_path.string();

    std::ifstream in_stream;
    in_stream.open(file_path.string());
    if (!in_stream.is_open()) {
        LOG(ERROR) << "can't open file:" << file_path;
        return false;
    }

    std::size_t file_size = static_cast<std::size_t>(get_file_size(file_path.c_str()));
    std::size_t current_kvs_pos = kvs.size();
    kvs.resize(kvs.size() + file_size);
    in_stream.read((char *)(&kvs[current_kvs_pos]), file_size);
    return !in_stream.fail();
}

bool Storage::load(const std::string &kv_dir) {
    try {
        fs::path path(kv_dir);
        if (!fs::exists(path)) {
            LOG(ERROR) << "path not exists:" << path;
            return false;
        }

        if (!fs::is_directory(path)) {
            LOG(ERROR) << "path not a directory:" << path;
            return false;
        }

        fs::directory_iterator end_iter;

        // Get the total size of kvs and pre-allocate the memory
        std::size_t total_size = 0;
        std::vector<byte> kvs;
        for (fs::directory_iterator dir_itr(path); dir_itr != end_iter; ++dir_itr) {
            total_size += get_file_size(dir_itr->path().c_str());
        }
        kvs.reserve(total_size);

        for (fs::directory_iterator dir_itr(path); dir_itr != end_iter; ++dir_itr) {
            if (!load_kv_file(dir_itr->path(), kvs)) {
                LOG(ERROR) << "failed to load model file: " << dir_itr->path();
                return false;
            }
        }

        _store.load_kvs(std::move(kvs));
    } catch (const fs::filesystem_error& ex) {
        LOG(ERROR) << ex.what();
        return false;
    }

    return true;
}

const byte *Storage::seek(const byte *key) const {
    return _store.seek(key);
}

}
}