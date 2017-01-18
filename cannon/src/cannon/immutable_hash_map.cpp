//
// Created by LuoHuaqing on 12/12/2016.
//

#include "immutable_hash_map.h"
#include "bytes_hash_functions.h"

namespace paradigm4 {
namespace cannon {

void ImmutableHashMap::load_kvs(std::vector<byte> &&kvs) {
    _table = std::move(kvs);
    construct_immutable_hash_map();
}

// Get the first power of 2 that is equal or greater than "size"
static inline std::size_t next_power(std::size_t size) {
    constexpr std::size_t hash_table_initial_size = 16;
    std::size_t i = hash_table_initial_size;

    while (i < size) {
        i *= 2;
    }
    return i;
}

void ImmutableHashMap::swap_kv(std::size_t x_index, std::size_t y_index) {
    if (x_index == y_index) return;

    byte tmp[_kv_size];
    void *x_p = &_table[x_index * _kv_size];
    void *y_p = &_table[y_index * _kv_size];

    memcpy(tmp, x_p, _kv_size);
    memcpy(x_p, y_p, _kv_size);
    memcpy(y_p, tmp, _kv_size);
}

void ImmutableHashMap::construct_immutable_hash_map() {
    _kv_count = _table.size() / _kv_size;
    std::size_t hash_size = next_power(_kv_count);
    _indexes.resize(hash_size + 1, 0);
    _hash_mask = hash_size - 1;

    for (std::size_t i = 0; i < _table.size(); i += _kv_size) {
        std::size_t hash = (__murmur2_or_cityhash<std::size_t>()(&_table[i], _key_size) & _hash_mask);
        // Use _indexes to store the number of collisions
        ++_indexes[hash];
    }

    std::size_t current_index = 0;
    for (std::size_t &index : _indexes) {
        std::size_t next_index = current_index + index;
        index = current_index;
        current_index = next_index;
    }

    std::vector<std::size_t> end_indexes(_indexes);
    for (std::size_t i = 0; i < _kv_count; ) {
        std::size_t hash = (__murmur2_or_cityhash<std::size_t>()(&_table[i * _kv_size], _key_size) & _hash_mask);
        std::size_t begin_index = _indexes[hash];
        std::size_t end_index = end_indexes[hash];
        if (i >= begin_index && i < end_index) {
            ++i;
        } else {
            swap_kv(i, end_index);
            ++end_indexes[hash];
        }
    }
}

const byte *ImmutableHashMap::seek(const byte *key) const {
    std::size_t hash = (__murmur2_or_cityhash<std::size_t>()(key, _key_size) & _hash_mask);
    std::size_t begin_index = _indexes[hash];
    std::size_t end_index = _indexes[hash + 1];
    while (begin_index < end_index) {
        if (memcmp(&_table[begin_index * _kv_size], key, _key_size) == 0) {
            return &_table[begin_index * _kv_size + _key_size];
        }
        ++begin_index;
    }

    return nullptr;
}

}
}