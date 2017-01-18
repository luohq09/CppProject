//
// Created by LuoHuaqing on 21/11/2016.
//

#ifndef CANNON_KV_SCHEMA_H
#define CANNON_KV_SCHEMA_H

#include <string>

namespace paradigm4 {
namespace cannon {
class KVSchema {
public:
    KVSchema(const std::string &schema_json_path);

    std::size_t get_key_size() const {
        return key_size;
    }

    std::size_t get_value_size() const {
        return value_size;
    }

private:
    std::size_t key_size;
    std::size_t value_size;
};
}
}

#endif //CANNON_KV_SCHEMA_H
