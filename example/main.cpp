#include <iostream>
#include <unistd.h>
#include "../src/events.hpp"

void printWithSleep(int mseconds, const std::wstring& line, bool with_endl = false) {
    usleep(mseconds);
    if (with_endl) std::wcout << std::endl;
    std::wcout << line;
    std::wcout.flush();
}

int main() {
    // Create the event handler
    Events<std::wstring> events;

    // Add callbacks
    events.add(L"greet", [](void*){
        printWithSleep(1000000, L"First ");
    }).add(L"greet", [](void*) {
        printWithSleep(1000000, L"First ");
    }).add(L"greet", [](void*) {
        printWithSleep(1000000, L"First ");
    }).add(L"greet", [](void*) {
        printWithSleep(1000000, L"First ");
    }).add(L"something", [](void*) {
        printWithSleep(1000000, L"I'm something. ", true);
    }).add(L"something", [](void* param) {
        if (param == nullptr) return;
        printWithSleep(0, *(std::wstring*)param + L".", true);
    });

    // Call all functions in "greet"-group using 2 threads
    events.emit(L"greet", 2);

    // Run functions in "something" in a single thread, with no parameter.
    // Notice that the last callback is fails the condition.
    events.emit(L"something");

    std::wstring* wstr = new std::wstring(L"I'm a parameter");
    events.emit(L"something", (void*)wstr);
    delete wstr;
    return 0;
}
