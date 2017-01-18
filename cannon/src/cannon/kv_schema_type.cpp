//
// Created by LuoHuaqing on 21/11/2016.
//

#include <vector>
#include <boost/algorithm/string.hpp>

#include "kv_schema_type.h"

namespace paradigm4 {
namespace cannon {

std::unordered_map<std::string, std::size_t> KVSchemaType::type_size_map = {
    {"i32", 4},
    {"i64", 8},
    {"float", 4},
    {"double", 8}
};

KVSchemaType::KVSchemaType(const std::string &type_string) throw() {
    type_size = 0;

    /*const std::string type = boost::to_lower_copy(
            boost::trim_copy(type_string));*/
    std::vector<std::string> type_count;
    boost::split(type_count, type_string, boost::is_any_of("["));

    if (type_count.empty()) {
        std::string err = "Failed to parse schema: " + type_string;
        throw std::runtime_error(err);
    }

    std::string type = boost::to_lower_copy(boost::trim_copy(type_count.front()));
    auto it = type_size_map.find(type);
    if (it == type_size_map.end()) {
        std::string err =  "Unknown type: " + type_count.front();
        throw std::runtime_error(err);
    }

    std::size_t element_size = it->second;
    std::size_t element_count = 1;

    if (type_count.size() > 1) {
        try {
            std::string count = boost::trim_copy(type_count[1]);
            element_count = std::stoull(count);
        } catch (...) {
            std::string err = "Failed to convert string (std::stoull): " + type_count[1];
            throw std::runtime_error(err);
        }
    }

    type_size = element_size * element_count;
}

std::size_t KVSchemaType::get_size() {
    return type_size;
}

}
}