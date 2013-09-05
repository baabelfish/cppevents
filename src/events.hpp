#pragma once

#include <iostream>
#include "threads.hpp"
#include <functional>
#include <unordered_map>
#include <thread>
#include <vector>
#include <stack>


template <typename KeyType>
class Events {
public:
    Events():
        m_functions() {
    }

    virtual ~Events() {
    }

    void emit(KeyType key, size_t threads) const {
        auto range = m_functions.equal_range(key);
        std::stack<std::function<void ()>> stack;
        for (auto it = range.first; it != range.second; ++it) {
            stack.push(it->second);
        }
        Threads tthreads(stack, threads);
        tthreads.execute();
    }

    void emit(KeyType key) const {
        auto range = m_functions.equal_range(key);
        for (auto it = range.first; it != range.second; ++it) {
            (it->second)();
        }
    }

    inline Events& add(KeyType key, std::function<void ()> func) {
        std::pair<KeyType, std::function<void ()>> mpair(key, func);
        m_functions.insert(mpair);
        return *this;
    }

private:
    std::unordered_multimap<KeyType, std::function<void ()>> m_functions;
};
