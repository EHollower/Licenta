#pragma once

#include <chrono>

namespace TLE {
    namespace {
        inline std::chrono::time_point <std::chrono::high_resolution_clock> start_time;
        constexpr long long TIME_LIMIT_MS = 5'000;
    }

    inline void start() {
        start_time = std::chrono::high_resolution_clock::now();
    }

    inline long long elapsed() {
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast <std::chrono::milliseconds>(now - start_time).count();
    }

    inline bool TLE() {
        return elapsed() > TIME_LIMIT_MS;
    }
}