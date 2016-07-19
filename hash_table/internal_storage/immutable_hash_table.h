//
// Created by LuoHuaqing on 7/19/16.
//

#ifndef IMMUTABLE_HASH_TABLE_IMMUTABLE_HASH_TABLE_H
#define IMMUTABLE_HASH_TABLE_IMMUTABLE_HASH_TABLE_H

#include <vector>
#include <utility>

namespace paradigm4 {
#define HASH_TABLE_INITIAL_SIZE 16
    static inline size_t _nextPower(size_t size) {
        size_t i = HASH_TABLE_INITIAL_SIZE;

        if (size >= LONG_MAX) return LONG_MAX;
        while (i < size) {
            i *= 2;
        }
        return i;
    }

    template<typename keyT, typename valT>
    class immutable_hash_table {
    public:
        typedef std::pair<keyT, valT> entryT;

        immutable_hash_table(std::vector<entryT> &kvs)
                : _table(std::move(kvs)) {
            size_t next_power = _nextPower(_table.size());
            _indexes.resize(next_power, 0);
            _size_mask = next_power - 1;

            for (const auto &kv : _table) {
                size_t h = std::hash<keyT>()(kv.first) & _size_mask;
                ++_indexes[h];
            }

            size_t index = 0;
            size_t i = 0;
            while (i < _indexes.size()) {
                size_t next_index = index + _indexes[i];
                _indexes[i] = index;
                index = next_index;
                ++i;
            }

            std::vector<size_t> cur_indexes(_indexes);
            i = 0;
            while (i < _table.size()) {
                entryT &entry = _table[i];
                size_t h = std::hash<keyT>()(entry.first) & _size_mask;
                size_t cur_index = cur_indexes[h];
                size_t start_index = _indexes[h];
                if (i >= start_index && i <= cur_index) {
                    ++i;
                } else {
                    std::swap(entry, _table[cur_index]);
                    ++cur_indexes[h];
                }
            }
        }

        const entryT *find(const keyT &key) const {
            size_t h = std::hash<keyT>()(key) & _size_mask;
            size_t s = _indexes[h];
            size_t e = (h == _size_mask ? _table.size() : _indexes[h + 1]);
            while (s < e) {
                if (_table[s].first == key) {
                    return &_table[s];
                }
                ++s;
            }

            return nullptr;
        }

    private:
        std::vector<entryT> _table;
        std::vector<size_t> _indexes;
        size_t _size_mask;
    };
}

#endif //IMMUTABLE_HASH_TABLE_IMMUTABLE_HASH_TABLE_H
