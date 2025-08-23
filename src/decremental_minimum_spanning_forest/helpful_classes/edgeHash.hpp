#pragma once

#include <tuple>
#include <functional>

template<typename Key>
using Edge = std::tuple<Key, Key, int>;

template<typename Key>
struct EdgeHash {
    size_t operator()(const Edge<Key>& e) const {
        auto h1 = std::hash<Key>{}(std::get<0>(e));
        auto h2 = std::hash<Key>{}(std::get<1>(e));
        auto h3 = std::hash<int>{}(std::get<2>(e));
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

struct EdgeKey {
    int u, v;

    bool operator==(const EdgeKey& other) const {
        return u == other.u && v == other.v;
    }
};
struct EdgeKeyHash {
    size_t operator()(const EdgeKey& k) const noexcept {
        return (static_cast<size_t>(k.u) << 32) ^ static_cast<size_t>(k.v);
    }
};
