#include "storage.h"

#include <fstream>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <glog/logging.h>

namespace paradigm4 {
namespace cannon {

namespace fs = boost::filesystem;

inline int get_index(int data_version) {
    return (data_version & 1);
}

Storage::Storage() {}

Storage::~Storage() {
    destroy();
}

void Storage::init() {
    // do nothing
}

void Storage::destroy() {
    // do nothing
}

size_t Storage::size(int data_version) const {
    return _stores[get_index(data_version)].size();
}

Storage::Status Storage::seek(Storage::TKey key, Storage::TValue* value, int data_version) const {
    DLOG(INFO) << "looking for key: " << key << " with data version: " << data_version;
    std::cout << "looking for key: " << key << " with data version: " << data_version <<std::endl;
    int store_idx = get_index(data_version);
    auto it = _stores[store_idx].find(key);
    if (it == _stores[store_idx].end()) {
        DLOG(INFO) << "key not found: " << key;
        return NOT_FOUND;
    }

    *value = it->second;
    DLOG(INFO) << "key found:" << key << " " << "value:" << *value;
    return FOUND;
}

bool Storage::insert(TKey key, TValue value, int data_version) {
    DLOG(INFO) << "insert kv: (" << key << ", "<<  value  << ") " << "with data version: " << data_version;
    std::cout << "insert kv: (" << key << ", "<<  value  << ") " << "with data version: " << data_version <<std::endl;
    int store_idx = get_index(data_version);
    auto it = _stores[store_idx].insert(std::make_pair(key, value));
    if (!it.second) {
        DLOG(FATAL) << "fail to insert kv: (" << key << ", "<<  value << ")";
    }

    return it.second;
}

void Storage::clear_data(int data_version) {
    // ToDo: need to test if the "swap" really free the memory
    TStore tmp;
    _stores[get_index(data_version)].swap(tmp);
}

} // namespace cannon
} // namespace paradigm4
