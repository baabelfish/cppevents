#include <iostream>
#include <unistd.h>
#include "../src/events.hpp"

int main() {
    // Create the event handler
    Events<std::wstring> events;

    // Add callbacks
    events.add(L"greet", [](void*){
        sleep(1);
        std::wcout << L"First ";
        std::wcout.flush();
    }).add(L"greet", [](void*) {
        sleep(1);
        std::wcout << L"Second ";
        std::wcout.flush();
    }).add(L"greet", [](void*) {
        sleep(1);
        std::wcout << L"Third ";
        std::wcout.flush();
    }).add(L"greet", [](void*) {
        sleep(1);
        std::wcout << L"Fourth ";
        std::wcout.flush();
    }).add(L"something", [](void*) {
        std::wcout << std::endl << L"I'm something. ";
    }).add(L"something", [](void* param) {
        if (param == nullptr) return;
        usleep(100);
        std::wstring* wstr = (std::wstring*)param;
        std::wcout << *wstr << L".";
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
