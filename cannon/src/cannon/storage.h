//
// Created by LuoHuaqing on 14/12/2016.
//

#ifndef CANNON_STORAGE_H
#define CANNON_STORAGE_H


#include <vector>
#include <boost/filesystem/path.hpp>

#include "immutable_hash_map.h"
#include "kv_schema.h"

namespace paradigm4 {
namespace cannon {
namespace fs = boost::filesystem;

class Storage {
public:
    Storage(const KVSchema &kvSchema): _store(kvSchema.get_key_size(), kvSchema.get_value_size()) {
    }

    std::size_t size() const {
        return _store.size();
    }

    std::size_t get_key_size() const {
        return _store.get_key_size();
    }

    std::size_t get_value_size() const {
        return _store.get_value_size();
    }

    bool load(const std::string &kv_dir);
    const byte *seek(const byte *key) const; // return nullptr when failing to seek the key

private:
    bool load_kv_file(const fs::path& file_path, std::vector<byte> &kvs);
private:
    ImmutableHashMap _store;
};

}
}


#endif //CANNON_STORAGE_H
