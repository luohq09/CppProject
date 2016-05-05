// author: selevent(zhanzhenjiang@4paradigm.com)

#ifndef PARADIGM4_CANNON_STORAGE_H
#define PARADIGM4_CANNON_STORAGE_H

#include <string>
#include <memory>
#include <boost/filesystem.hpp>

#include <sparsehash/sparse_hash_map>
// #include "immutable_hash_table.h"

namespace paradigm4 {
namespace cannon {

class Storage {
public:
    typedef int64_t TKey;
    typedef double  TValue;
    typedef google::sparse_hash_map<TKey, TValue> TStore;
    typedef std::shared_ptr<TStore> TStorePtr;

public:
    enum Status {
        FOUND,
        NOT_FOUND,
        ERROR 
    };

public:
    Storage();
    ~Storage();
    void init();
    void destroy();
    size_t size(int data_version) const;
    Status seek(TKey key, TValue* value, int data_version) const;
    bool insert(TKey key, TValue value, int data_version);
    void clear_data(int data_version);

private:
    bool is_reloading() const;

private:
    TStore _stores[2];
};

} // namespace cannon
} // namespace paradigm4

#endif // PARADIGM4_CANNON_STORAGE_H
