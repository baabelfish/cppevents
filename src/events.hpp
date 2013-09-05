#pragma once

#include "threads.hpp"
#include <functional>
#include <unordered_map>
#include <stack>

template <class KeyType, class ParameterType>
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
    void emit(KeyType key, size_t threads, ParameterType parameter) const {
        auto range = m_functions.equal_range(key);
        std::stack<std::function<void (ParameterType)>> stack;
        for (auto it = range.first; it != range.second; ++it) {
            stack.push(it->second);
        }
        Threads<ParameterType> tthreads(stack, threads);
        tthreads.execute(parameter);
    }

    /**
     * @brief Runs key-functions in a single thread.
     *
     * @param key Key of the signal.
     * @param parameter Optional parameter to the function.
     */
    void emit(KeyType key, ParameterType parameter) const {
        auto range = m_functions.equal_range(key);
        for (auto it = range.first; it != range.second; ++it) {
            (it->second)(parameter);
        }
    }

    /**
     * @brief Adds a callback to a keycontainer.
     *
     * @param key Key of the event.
     * @param func Function to run. Takes a ParameterType parameter.
     *
     * @return Reference to this instance to be able to chain calls.
     */
    Events& add(KeyType key, std::function<void (ParameterType)> func) {
        std::pair<KeyType, std::function<void (ParameterType)>> mpair(key, func);
        m_functions.insert(mpair);
        return *this;
    }

private:
    /**
     * @brief Hash table to hold the functions.
     */
    std::unordered_multimap<KeyType, std::function<void (ParameterType)>> m_functions;
};
