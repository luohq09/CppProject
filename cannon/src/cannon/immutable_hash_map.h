//
// Created by LuoHuaqing on 12/12/2016.
//

#ifndef CANNON_IMMUTABLE_HASH_MAP_H
#define CANNON_IMMUTABLE_HASH_MAP_H

#include <cstddef>
#include <cstdint>
#include <vector>

namespace paradigm4 {
namespace cannon {

typedef int8_t byte;

class ImmutableHashMap {
public:
    ImmutableHashMap(std::size_t key_size, std::size_t value_size):
            _key_size(key_size),
            _value_size(value_size),
            _kv_size(key_size + value_size) {
        _kv_count = 0;
    }

    void load_kvs(std::vector<byte> &&kvs);
    const byte *seek(const byte *key) const; // return nullptr when failing to seek the key

    std::size_t get_key_size() const {
        return _key_size;
    }

    std::size_t get_value_size() const {
        return _value_size;
    }

    std::size_t size() const {
        return _kv_count;
    }

private:
    void construct_immutable_hash_map();
    void swap_kv(std::size_t x_index, std::size_t y_index);
private:
    const std::size_t _key_size;
    const std::size_t _value_size;
    const std::size_t _kv_size; // _key_size + _value_size

    std::size_t _kv_count;
    std::size_t _hash_mask;

    std::vector<byte> _table;
    std::vector<std::size_t> _indexes;
};

}
}

#endif //CANNON_IMMUTABLE_HASH_MAP_H
