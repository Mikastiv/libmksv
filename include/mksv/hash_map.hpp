#pragma once

#include "array_list.hpp"
#include "hash.hpp"

namespace mksv {

template <typename K, typename T>
struct HashMap {
    struct Entry {
        K key;
        T value;
    };

    struct Iterator {
        ArrayList<ArrayList<Entry>>* elems;
        u64 bucket_idx;
        u64 entry_idx;

        Entry&
        operator*() const {
            return (*elems)[bucket_idx][entry_idx];
        }

        Iterator&
        operator++() {
            const u64 size = (*elems)[bucket_idx].size;
            if (entry_idx + 1 < size) {
                ++entry_idx;
                return *this;
            }

            u64 i = bucket_idx + 1;
            while (i < (*elems).size) {
                if ((*elems)[i].size != 0) {
                    bucket_idx = i;
                    entry_idx = 0;
                    return *this;
                }
                ++i;
            }

            bucket_idx = (*elems).size;
            entry_idx = 0;
            return *this;
        }

        Iterator
        operator++(int) const {
            Iterator it = *this;
            ++(*this);
            return it;
        }

        bool
        operator==(const Iterator& other) const {
            return bucket_idx == other.bucket_idx && entry_idx == other.entry_idx;
        }

        bool
        operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };

    mem::Allocator allocator;
    ArrayList<ArrayList<Entry>> elems;
    u64 size;
    u64 first;

    [[nodiscard]] static bool
    init(const mem::Allocator allocator, const u64 initial_size, HashMap* out_map) {
        HashMap map = {
            .allocator = allocator,
            .elems = ArrayList<ArrayList<Entry>>::init(allocator),
            .size = 0,
        };

        if (!map.elems.ensure_capacity(initial_size)) return false;
        for (u64 idx = 0; idx < initial_size; ++idx) {
            if (!map.elems.append(ArrayList<Entry>::init(allocator))) return false;
        }

        map.first = map.elems.size;
        *out_map = map;

        return true;
    }

    // overrides value at key is present
    [[nodiscard]] bool
    insert(const K key, const T value) {
        if (should_grow()) {
            if (!grow()) return false;
        }

        const u64 idx = hash(key);
        for (auto& entry : elems[idx]) {
            if (entry.key == key) {
                entry.value = value;
                return true;
            }
        }

        if (!elems[idx].append(Entry{ key, value })) return false;

        if (idx < first) first = idx;

        ++size;

        return true;
    }

    [[nodiscard]] bool
    find(const K key, T* out_value) const {
        const u64 idx = hash(key);
        for (const auto entry : elems[idx]) {
            if (entry.key == key) {
                *out_value = entry.value;
                return true;
            }
        }
        return false;
    }

    void
    clear() {
        for (auto& e : elems) {
            e.clear();
        }
    }

    Iterator
    begin() {
        return {
            .elems = &elems,
            .bucket_idx = first,
            .entry_idx = 0,
        };
    }

    Iterator
    begin() const {
        return {
            .elems = &elems,
            .bucket_idx = first,
            .entry_idx = 0,
        };
    }

    Iterator
    end() {
        return {
            .elems = &elems,
            .bucket_idx = elems.size,
            .entry_idx = 0,
        };
    }

    Iterator
    end() const {
        return {
            .elems = &elems,
            .bucket_idx = elems.size,
            .entry_idx = 0,
        };
    }

private:
    u64
    hash(const K key) const {
        return hash::hash({ (u8*)&key, sizeof(K) }) % elems.size;
    }

    bool
    should_grow() const {
        const f32 ratio = (f32)size / (f32)elems.size;
        return ratio >= 0.8;
    }

    void
    free() {
        for (u64 idx = 0; idx < size; ++idx) {
            elems[idx].deinit();
        }
        elems.deinit();
    }

    [[nodiscard]] bool
    grow() {
        HashMap new_hmap;
        if (!init(allocator, size * 2, &new_hmap)) return false;

        for (const auto& e : *this) {
            if (!new_hmap.insert(e.key, e.value)) return false;
        }

        free();
        *this = new_hmap;

        return true;
    }
};

} // namespace mksv
