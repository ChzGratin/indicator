#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <thread>
#include <chrono>
#include <stdio.h>

namespace indicator
{
    // hide cursor
    void hideCursor() { printf("\e[?25l"); }

    // show cursor
    void showCursor() { printf("\e[?25h"); }
    
    // avr-style delay function
    void delay_ms(int t_ms)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(t_ms));
    }
}

#endif