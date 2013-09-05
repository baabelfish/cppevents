#pragma once

#include "threads.hpp"
#include <functional>
#include <unordered_map>
#include <stack>

template <class KeyType>
class Events {
public:
    /**
     * @brief Creates an event handler.
     */
    Events();

    /**
     * @brief Destroys the event.
     */
    virtual ~Events();

    /**
     * @brief Emits a signal identified by the key.
     *
     * @param key Key of the signal.
     * @param threads Amount of threads to use in execution.
     * @param parameter Optional parameter to the function.
     */
    template <class ParameterType>
    void emit(KeyType key, size_t threads, ParameterType parameter) const;

    /**
     * @brief Runs key-functions in a single thread.
     *
     * @param key Key of the signal.
     * @param parameter Optional parameter to the function.
     */
    template <class ParameterType>
    void emit(KeyType key, ParameterType parameter) const;

    /**
     * @brief Adds a callback to a keycontainer.
     *
     * @param key Key of the event.
     * @param func Function to run. Takes a ParameterType parameter.
     *
     * @return Reference to this instance to be able to chain calls.
     */
    template <class ParameterType>
    Events& add(KeyType key, std::function<void (ParameterType)> func);

private:
    template <class ParameterType>
    static std::unordered_multimap<KeyType, std::function<void (ParameterType)>>& tcontainer() {
        static std::unordered_multimap<KeyType, std::function<void (ParameterType)>> functions;
        return functions;
    }
};

template <class KeyType>
Events<KeyType>::Events() {
}

template <class KeyType>
Events<KeyType>::~Events() {
}

template <class KeyType>
template <class ParameterType>
void Events<KeyType>::emit(KeyType key, ParameterType parameter) const {
    auto range = tcontainer<ParameterType>().equal_range(key);
    for (auto it = range.first; it != range.second; ++it) {
        (it->second)(parameter);
    }
}

template <class KeyType>
template <class ParameterType>
Events<KeyType>& Events<KeyType>::add(KeyType key, std::function<void (ParameterType)> func) {
    static std::unordered_multimap<KeyType, std::function<void (ParameterType)>> functions;
    std::pair<KeyType, std::function<void (ParameterType)>> mpair(key, func);
    tcontainer<ParameterType>().insert(mpair);
    return *this;
}

template <class KeyType>
template <class ParameterType>
void Events<KeyType>::emit(KeyType key, size_t threads, ParameterType parameter) const {
    auto range = tcontainer<ParameterType>().equal_range(key);
    std::stack<std::function<void (ParameterType)>> stack;
    for (auto it = range.first; it != range.second; ++it) {
        stack.push(it->second);
    }
    Threads<ParameterType> tthreads(stack, threads);
    tthreads.execute(parameter);
}
