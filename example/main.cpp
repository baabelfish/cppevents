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
    events.add<std::wstring>(L"greet", [](std::wstring){
        printWithSleep(1000000, L"First ");
    }).add<std::wstring>(L"greet", [](std::wstring) {
        printWithSleep(1000000, L"Second ");
    }).add<std::wstring>(L"greet", [](std::wstring) {
        printWithSleep(1000000, L"Third ");
    }).add<std::wstring>(L"greet", [](std::wstring) {
        printWithSleep(1000000, L"Fourth ");
    }).add<std::wstring>(L"something", [](std::wstring) {
        printWithSleep(1000000, L"I'm something. ", true);
    }).add<std::wstring>(L"something", [](std::wstring param) {
        if (param.empty()) return;
        printWithSleep(0, param + L".", true);
    }).add<int>(L"intit", [](int param) {
        std::wcout << param << std::endl;
    });

    events.emit<int>(L"intit", 5);

    // Call all functions in "greet"-group using 2 threads
    events.emit<std::wstring>(L"greet", 2, L"");

    // Run functions in "something" in a single thread, with no parameter.
    // Notice that the last callback is fails the condition.
    events.emit(L"something", L"");

    // With an actual parameter
    events.emit(L"something", L"I'm a parameter");
    return 0;
}
