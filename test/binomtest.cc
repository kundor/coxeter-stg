#define BINOM_ONLY
#define BINOM_CHECK
#include "../binom.cc"
#include <cstdio>
#include <limits>
#include <gmp.h>
using std::printf;
using std::putchar;

uint64_t badbinom(int n, int k) {
    if (k < 0 || n < k) // also covers n < 0
        return 0;
    if (k > n/2)
        k = n - k;
    uint64_t result = 1;
    for (uint64_t i = 1; i <= static_cast<unsigned>(k); ++i) {
        result *= (n - k + i);
        result /= i;
    }
    return result;
}

int main() {
    int maxn = 12;
    for (int n = 0; n <= maxn; ++n) {
        printf("%*s", 2*maxn - 2*n, "");
        for (int k = 0; k <= n; ++k) {
            printf("%3llu ", binom(n,k));
        }
        putchar('\n');
    }
    printf("-2 C 3: %llu\n", binom(-2,3));
    printf("2 C 3: %llu\n", binom(2,3));
    printf("2 C -1: %llu\n", binom(2,-1));
    printf("30 C 15: %20llu\n", binom(30,15)); // 155117520
    printf("60 C 30: %20llu\n", binom(60,30)); // 1.1826458156e17
    printf("70 C 27: %20llu\n", binom(70,27)); // 1.820855884e19
    // should barely fit in an unsigned long

    // This won't fit in a long
    try {
        printf("99 C 60: %20llu\n", binom(120,60)); // 5.498493658e27
    } catch (const std::overflow_error& e) {
        printf("99 C 60: %s\n", e.what());
    }

    printf("2147483647 C 1: %llu\n", binom(2147483647,1));
    printf("2147483647 C 2: %llu\n", binom(2147483647,2));

    maxn = std::numeric_limits<int>::max();
    int mink = maxn, bargct = 0/*, minbargk = maxn*/;
    mpz_t c;
    mpz_init(c);

    for (int n = 13; n < maxn; ++n) {
        for (int k = 0; k <= n/2; ++k) {
            if (k >= mink) break;
            mpz_bin_uiui(c, n, k);
            if (mpz_fits_ulong_p(c)) {
                auto d = binom(n, k);
                if (d != mpz_get_ui(c))
                    gmp_printf("AAAGH, %d C %d = %llu vs. %Zd\n", n, k, d, c);
                d = badbinom(n, k);
                if (d != mpz_get_ui(c)) {
                    ++bargct;
                   /* if (k < minbargk) {
                        minbargk = k;
                        int wid;
                        gmp_printf("Bargh, %d C %d = %Zd%n vs.\n",
                                     n, k, c, &wid);
                        printf("%*llu\n", wid, d);
                    }*/
                }
            } else {
                if (k < mink) {
                    mink = k;
                    printf("%d C %d doesn't fit\n", n, k);
                }
                break;
            }
        }
        if (n % (1 << 21) == 0) {
            putchar('.');
            fflush(stdout);
        }
    }
    printf("Wrong answers with naive algorithm: %d\n", bargct);
    // There are 1,522,323
    return 0;
}

