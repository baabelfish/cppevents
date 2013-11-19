#pragma once

#include "threads.hpp"
#include <functional>
#include <unordered_map>
#include <stack>
#include <string>

class Events {
public:
    static Events& instance() {
        static Events events;
        return events;
    }

    /**
     * @brief Emits a signal identified by the key.
     *
     * @param key Key of the signal.
     * @param threads Amount of threads to use in execution.
     * @param parameter Optional parameter to the function.
     */
    template <class ParameterType>
    void emit(std::string key, size_t threads, ParameterType parameter) const;
    void emit(std::string key, size_t threads) const;

    /**
     * @brief Adds a callback to a keycontainer.
     *
     * @param key Key of the event.
     * @param func Function to run. Takes a ParameterType parameter.
     *
     * @return Reference to this instance to be able to chain calls.
     */
    template <class ParameterType>
    Events& add(std::string key, std::function<void (ParameterType)> func);
    Events& add(std::string key, std::function<void ()> func);

protected:
    /**
        * @brief Creates an event handler.
        */
    Events();

    /**
        * @brief Destroys the event.
        */
    virtual ~Events();

private:
    template <class ParameterType>
    std::unordered_multimap<std::string, std::function<void (ParameterType)>>& tcontainer() const {
        static std::unordered_multimap<std::string, std::function<void (ParameterType)>> functions;
        return functions;
    }
    std::unordered_multimap<std::string, std::function<void ()>> m_functions;
};

Events::Events() {
}

Events::~Events() {
}

template <class ParameterType>
Events& Events::add(std::string key, std::function<void (ParameterType)> func) {
    static std::unordered_multimap<std::string, std::function<void (ParameterType)>> functions;
    std::pair<std::string, std::function<void (ParameterType)>> mpair(key, func);
    tcontainer<ParameterType>().insert(mpair);
    return *this;
}

template <class ParameterType>
void Events::emit(std::string key, size_t threads, ParameterType parameter) const {
    auto range = tcontainer<ParameterType>().equal_range(key);
    std::stack<std::function<void (ParameterType)>> stack;
    for (auto it = range.first; it != range.second; ++it) {
        if (threads == 0) (it->second)(parameter);
        else stack.push(it->second);
    }
    if (threads != 0) {
        Threads<ParameterType> tthreads(stack, threads);
        tthreads.execute(parameter);
    }
}

Events& Events::add(std::string key, std::function<void ()> func) {
    static std::unordered_multimap<std::string, std::function<void ()>> functions;
    std::pair<std::string, std::function<void ()>> mpair(key, func);
    m_functions.insert(mpair);
    return *this;
}
