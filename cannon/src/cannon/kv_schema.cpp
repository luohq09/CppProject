//
// Created by LuoHuaqing on 21/11/2016.
//
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "kv_schema.h"
#include "kv_schema_type.h"

namespace paradigm4 {
namespace cannon {

namespace pt = boost::property_tree;

KVSchema::KVSchema(const std::string &schema_json_path) {
    key_size = 0;
    value_size = 0;

    pt::ptree root;
    boost::property_tree::read_json(schema_json_path, root);

    for (pt::ptree::value_type &key : root.get_child("key")) {
        std::string type_string = key.second.get<std::string>("type");
        KVSchemaType type(type_string);
        key_size += type.get_size();
    }

    for (pt::ptree::value_type &value : root.get_child("value")) {
        std::string type_string = value.second.get<std::string>("type");
        KVSchemaType type(type_string);
        value_size += type.get_size();
    }
}

}
}