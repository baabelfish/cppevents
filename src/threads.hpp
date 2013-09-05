#pragma once

#include <functional>
#include <stack>
#include <vector>
#include <thread>
#include <mutex>

class Threads {
public:
    Threads(std::stack<std::function<void ()>> stack, size_t thread_amount = 2);
    virtual ~Threads();
    void execute();

private:
    size_t m_thread_amount;
    std::stack<std::function<void ()>> m_executions;
};
