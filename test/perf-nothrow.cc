// Prevent the inclusion of binom.h, and its definition of BINOM_CHECK
#define NAM_BINOM_H
#include <cstdint>
#include "../binom.cc"
#include <cstdio>
#include <chrono>

int main() {
    int n = 1;
    auto tic = std::chrono::high_resolution_clock::now();
    for (; n < 69; ++n) {
        for (int k = 0; k < 30; ++k) {
            auto d = binom(n, k);
            if (d == 71) // try to discourage optimizing stuff out
                std::putchar('.');
        }
    }
    for (; n < 87; ++n) {
        for (int k = 0; k < 20; ++k) {
            auto d = binom(n, k);
            if (d == 7) // try to discourage optimizing stuff out
                std::putchar('.');
        }
    }
    for (; n < 387; ++n) {
        for (int k = 0; k < 10; ++k) {
            auto d = binom(n, k);
            if (d == 7) // try to discourage optimizing stuff out
                std::putchar('.');
        }
    }
    for (; n < 18581; ++n) {
        for (int k = 0; k < 5; ++k) {
            auto d = binom(n, k);
            if (d == 7) // try to discourage optimizing stuff out
                std::putchar('.');
        }
    }
    auto toc = std::chrono::high_resolution_clock::now();
    std::printf("%lld µs\n",
        std::chrono::duration_cast<std::chrono::microseconds>(toc - tic).count());
    return 0;
}


