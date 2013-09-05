#include "threads.hpp"

#include <thread>
#include <mutex>
#include <vector>

Threads::Threads(std::stack<std::function<void (void*)>> stack, size_t thread_amount):
    m_thread_amount(thread_amount == 0 ? 1 : thread_amount)
    , m_executions(stack) {
}

Threads::~Threads() {
}

void Threads::execute(void* parameter) {
    if (m_executions.empty()) return;
    std::vector<std::thread*> threads;
    std::mutex mt;

    for (size_t i = 0; i < m_thread_amount; ++i) {
        threads.push_back(new std::thread([&]() {
            while (true) {
                std::function<void (void*)> element;
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
