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

template<typename Key>

struct PairHash {
    std::size_t operator()(const std::pair<Key, Key>& pair) const {
        auto h1 = std::hash<Key>{}(pair.first);
        auto h2 = std::hash<Key>{}(pair.second);
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};