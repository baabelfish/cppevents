#include <iostream>
#include <unistd.h>
// #include <chrono>
#include "../src/events.hpp"
// #include "../src/timer.hpp"

void printWithSleep(int mseconds, const std::wstring& line, bool with_endl = false) {
    usleep(mseconds);
    if (with_endl) std::wcout << std::endl;
    std::wcout << line;
    std::wcout.flush();
}

int main() {
    // Add callbacks
    Events::instance().add("greet", [](){
        printWithSleep(1000000, L"First ");
    }).add("greet", []() {
        printWithSleep(1000000, L"Second ");
    }).add("greet", []() {
        printWithSleep(1000000, L"Third ");
    }).add("greet", []() {
        printWithSleep(1000000, L"Fourth ");
    }).add("something", []() {
        printWithSleep(1000000, L"I'm something. ", true);
    }).add<std::wstring>("something", [](std::wstring param) {
        if (param.empty()) return;
        printWithSleep(0, param + L".", true);
    }).add<int>("intit", [](int param) {
        std::wcout << 2 * param << std::endl;
    }).add<int>("intit", [](int param) {
        std::wcout << param << std::endl;
    });

    Events::instance().emit<int>("intit", 5, 5);
    // Events::instance().emit("greet", 2);
    // events.emit(L"greet");
    // events2.emit(L"greet");

    // Timer timer;
    // timer.runAfter(3000, []() {
    //         std::cout << "Toka" << std::endl;
    //         });
    // timer.runAfter(2000, []() {
    //         std::cout << "Eka" << std::endl;
    //         });
    // timer.runAfter(6000, []() {
    //         std::cout << "Kolmas" << std::endl;
    //         });

    while (true) {
    }

    // std::chrono::steady_clock::time_point tt =
    // std::chrono::steady_clock::time_point tt = std::chrono::hours(5);// std::chrono::steady_clock::now();
    // std::cout << tt.time_since_epoch().count() << std::endl;
    // std::cout << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::hours(1)).count() << std::endl;


    // Call all functions in "greet"-group using 2 threads
    // events.emit<std::wstring>(L"something", 2, L"I'm a parameter");
    // events.emit(L"greet");
    // events.emit<std::wstring>(L"something", L"I'm a parameter");

    // Run functions in "something" in a single thread, with no parameter.
    // Notice that the last callback is fails the condition.
    // events.emit(L"something", L"");

    // With an actual parameter
    // events.emit(L"something", L"I'm a parameter");
    return 0;
}
