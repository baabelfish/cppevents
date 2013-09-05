#pragma once

#include "threads.hpp"
#include <functional>
#include <unordered_map>
#include <stack>


template <typename KeyType>
class Events {
public:
    /**
     * @brief Creates an event handler.
     */
    Events():
        m_functions() {
    }

    /**
     * @brief Destroys the event.
     */
    virtual ~Events() {
    }

    /**
     * @brief Emits a signal identified by the key.
     *
     * @param key Key of the signal.
     * @param threads Amount of threads to use in execution.
     * @param parameter Optional parameter to the function.
     */
    void emit(KeyType key, size_t threads, void* parameter = nullptr) const {
        auto range = m_functions.equal_range(key);
        std::stack<std::function<void (void*)>> stack;
        for (auto it = range.first; it != range.second; ++it) {
            stack.push(it->second);
        }
        Threads tthreads(stack, threads);
        tthreads.execute(parameter);
    }

    /**
     * @brief Runs key-functions in a single thread.
     *
     * @param key Key of the signal.
     * @param parameter Optional parameter to the function.
     */
    void emit(KeyType key, void* parameter = nullptr) const {
        auto range = m_functions.equal_range(key);
        for (auto it = range.first; it != range.second; ++it) {
            (it->second)(parameter);
        }
    }

    /**
     * @brief Adds a callback to a keycontainer.
     *
     * @param key Key of the event.
     * @param func Function to run. Takes a void* parameter.
     *
     * @return Reference to this instance to be able to chain calls.
     */
    Events& add(KeyType key, std::function<void (void*)> func) {
        std::pair<KeyType, std::function<void (void*)>> mpair(key, func);
        m_functions.insert(mpair);
        return *this;
    }

private:
    /**
     * @brief Hash table to hold the functions.
     */
    std::unordered_multimap<KeyType, std::function<void (void*)>> m_functions;
};
