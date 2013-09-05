#include <iostream>
#include <unistd.h>
#include "../src/events.hpp"

int main() {
    Events<std::wstring> events;
    events.add(L"greet", [](){
        sleep(1);
        std::wcout << L"First ";
        std::wcout.flush();
    })
    .add(L"greet", []() {
        sleep(1);
        std::wcout << L"Second ";
        std::wcout.flush();
    })
    .add(L"greet", []() {
        sleep(1);
        std::wcout << L"Third ";
        std::wcout.flush();
    })
    .add(L"greet", []() {
        sleep(1);
        std::wcout << L"Fourth ";
        std::wcout.flush();
    })
    .add(L"something", []() {
        std::wcout << std::endl << L"I'll do the same" << std::endl;
    });

    events.emit(L"greet", 2);
    events.emit(L"something");
    return 0;
}
