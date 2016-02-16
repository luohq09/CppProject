//
// Created by LuoHuaqing on 2/15/16.
//

#ifndef CANNON_IMMUTABLE_HASH_TABLE_H
#define CANNON_IMMUTABLE_HASH_TABLE_H

#include <limits.h>
#include <stddef.h>
#include "hash_function.h"

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

    template <typename keyT, typename valT>
    struct tableEntry {
        keyT key;
        valT val;
        tableEntry *next;

        tableEntry() : next(nullptr) {}
        tableEntry(keyT k, valT v) : key(k), val(v), next(nullptr) {}
    };

    template <typename keyT, typename valT>
    class immutable_hash_table {
    public:
        typedef tableEntry<keyT, valT> tableEntryT;

        immutable_hash_table(size_t size = HASH_TABLE_INITIAL_SIZE) {
            size = _nextPower(size);
            _size = size;
            _used = 0;
            _size_mask = size - 1;
            _loading = false;

            _buckets = new tableEntryT *[size];
            for (size_t i = 0; i < size; ++i) {
                _buckets[i] = nullptr;
            }
        }

        ~immutable_hash_table() {
            _freeTable();
        }

        tableEntryT *find(const keyT &key) {
            if (_loading || _used == 0) {
                return nullptr;
            }

            size_t h = hash<keyT>{}(key);
            h = h & _size_mask;

            tableEntryT *head = _buckets[h];
            while (head != nullptr) {
                if (head->key == key) {
                    return head;
                }
            }

            return nullptr;
        }

        void beginLoading() {
            _reset();
            _loading = true;
        }

        void endLoading() {
            _shrinkIfNeeded();
            _loading = false;
        }

        tableEntryT *insert(keyT key, valT val) {
            // immutable: only insert data during 'loading'
            if (!_loading) {
                return nullptr;
            }

            _expandIfNeeded();

            size_t h = hash<keyT> {}(key);
            h = h & _size_mask;

            // Search if the bucket already contains the given key
            tableEntryT *head = _buckets[h];
            while (head != nullptr) {
                if (head->key == key) {
                    return nullptr;
                }

                head = head->next;
            }

            tableEntryT *entry = new tableEntryT(key, val);
            entry->next = _buckets[h];
            _buckets[h] = entry;
            ++_used;

            return entry;
        }

        size_t entryCount() {
            return _used;
        }

    private:
        tableEntryT **_buckets;
        size_t _size;
        size_t _size_mask;
        size_t _used;

        bool _loading;

        void _freeEntryList(size_t bucket) {
            tableEntryT *head = _buckets[bucket];
            while (head != nullptr) {
                tableEntryT *next = head->next;
                delete head;
                head = next;
            }

            _buckets[bucket] = nullptr;
        }

        void _freeTable() {
            for (size_t i = 0; i < _size; ++i) {
                _freeEntryList(i);
            }

            delete []_buckets;
        }

        void _reset() {
            for (size_t i = 0; i < _size; ++i) {
                _freeEntryList(i);
            }
            _used = 0;
            _loading = false;

        }

        void _expandIfNeeded() {
            // expand if we reached 1:1 ratio
            if (_used >= _size) {
                _rehash(_used * 2);
            }
        }

        void _shrinkIfNeeded() {
            if (_used * 2 < _size) {
                _rehash(_used * 2);
            }
        }

        bool _rehash(size_t size) {
            size = _nextPower(size);
            if (size == _size) {
                return false;
            }

            _size_mask = size - 1;
            tableEntryT **buckets = new tableEntryT *[size];

            // Full table rehash
            for (size_t i = 0; i < _size; ++i) {
                tableEntryT *head = _buckets[i];
                while (head != nullptr) {
                    size_t h = hash<keyT> {}(head->key);
                    h = h & _size_mask;
                    tableEntryT *next = head->next;
                    head->next = buckets[h];
                    buckets[h] = head;

                    head = next;
                }
            }
            _size = size;
            delete []_buckets;
            _buckets = buckets;
            return true;
        }
    };
}
#endif //CANNON_IMMUTABLE_HASH_TABLE_H
