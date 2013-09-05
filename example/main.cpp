#include <iostream>
#include <unistd.h>
#include "../src/events.hpp"

int main() {
    // Create the event handler
    Events<std::wstring> events;

    // Add callbacks
    events.add(L"greet", [](){
        sleep(1);
        std::wcout << L"First ";
        std::wcout.flush();
    }).add(L"greet", []() {
        sleep(1);
        std::wcout << L"Second ";
        std::wcout.flush();
    }).add(L"greet", []() {
        sleep(1);
        std::wcout << L"Third ";
        std::wcout.flush();
    }).add(L"greet", []() {
        sleep(1);
        std::wcout << L"Fourth ";
        std::wcout.flush();
    }).add(L"something", []() {
        std::wcout << std::endl << L"I'll do the same" << std::endl;
    });

    // Call all functions in "greet"-group using 2 threads
    events.emit(L"greet", 2);

    // Run functions in "something" in a single thread
    events.emit(L"something");
    return 0;
}
