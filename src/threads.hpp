#pragma once

#include <functional>
#include <stack>
#include <vector>
#include <thread>
#include <mutex>

class Threads {
public:
    /**
     * @brief Creates a thread runner.
     *
     * @param stack A stack with all the functions that need to be run.
     * @param thread_amount Amount of threads to use. Defaults to 2.
     */
    Threads(std::stack<std::function<void ()>> stack, size_t thread_amount = 2);

    /**
     * @brief Destroys the class.
     */
    virtual ~Threads();

    /**
     * @brief Executes all functions that were created. This works only once
     * in the instances existance.
     */
    void execute();

private:
    /**
     * @brief Amount of threads to use.
     */
    size_t m_thread_amount;

    /**
     * @brief Container for the functions to execute.
     */
    std::stack<std::function<void ()>> m_executions;
};
