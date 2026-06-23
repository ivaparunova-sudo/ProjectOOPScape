#pragma once
#include <iostream>

namespace Color {
    enum Code {
        RESET = 0,
        BRIGHT_RED = 91,
        BRIGHT_GREEN = 92,
        BRIGHT_YELLOW = 93,
        BRIGHT_CYAN = 96,
        DARK_GRAY = 90,
        WHITE = 97,
    };

    inline void set(Code c) {
        std::cout << "\033[" << (int)c << "m";
    }
    inline void reset() {
        std::cout << "\033[0m";
    }
};
