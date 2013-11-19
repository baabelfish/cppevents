#pragma once

#include <functional>
#include <stack>
#include <thread>
#include <mutex>
#include <vector>

template<class ParameterType>
class Threads {
public:
    /**
     * @brief Creates a thread runner.
     *
     * @param stack A stack with all the functions that need to be run.
     * @param thread_amount Amount of threads to use. Defaults to 2.
     */
    Threads::Threads(std::stack<std::function<void (ParameterType)>> stack, size_t thread_amount):
        m_thread_amount(thread_amount == 0 ? 1 : thread_amount)
        , m_executions(stack) {
    }

    /**
     * @brief Destroys the class.
     */
    virtual ~Threads();

    /**
     * @brief Executes all functions that were created. This works only once
     * in the instances existance.
     */
    void execute(ParameterType parameter);

private:
    /**
     * @brief Amount of threads to use.
     */
    size_t m_thread_amount;

    /**
     * @brief Container for the functions to execute.
     */
    std::stack<std::function<void (ParameterType)>> m_executions;
};

Threads::~Threads() {
}

void Threads::execute(ParameterType parameter) {
    if (m_executions.empty()) return;
    std::vector<std::thread*> threads;
    std::mutex mt;

    for (size_t i = 0; i < m_thread_amount; ++i) {
        threads.push_back(new std::thread([&]() {
            while (true) {
                std::function<void (ParameterType)> element;
                bool got_it = false;

                mt.lock();
                if (!m_executions.empty()) {
                    element = m_executions.top();
                    m_executions.pop();
                    got_it = true;
                } else {
                    mt.unlock();
                    break;
                }
                mt.unlock();

                if (got_it) {
                    element(parameter);
                }
            }
        }));
    }

    for (size_t i = 0; i < m_thread_amount; ++i) {
        threads[i]->join();
        delete threads[i];
    }
}
