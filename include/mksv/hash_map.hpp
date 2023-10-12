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

    mem::Allocator allocator;
    ArrayList<ArrayList<Entry>> elems;
    u64 size;

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

        return elems[idx].append(Entry{ key, value });
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

    // TODO
    [[nodiscard]] bool
    grow() {
        return true;
    }
};

} // namespace mksv
