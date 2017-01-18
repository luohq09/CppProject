//
// Created by LuoHuaqing on 21/11/2016.
//

#ifndef CANNON_KV_SCHEMA_TYPE_H
#define CANNON_KV_SCHEMA_TYPE_H

#include <string>
#include <unordered_map>

namespace paradigm4 {
namespace cannon {
class KVSchemaType {
public:
    KVSchemaType(const std::string &type_string) throw();
    std::size_t get_size();

private:
    std::size_t type_size;
    static std::unordered_map<std::string, std::size_t> type_size_map;
};
}
}


#endif //CANNON_KV_SCHEMA_TYPE_H
