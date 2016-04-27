#include "../binom.cc"
#include <cstdio>
#include <limits>
#include <gmp.h>
using std::printf;
using std::putchar;

int main() {
    int maxn = 12;
    for (int n = 0; n <= maxn; ++n) {
        printf("%*s", 2*maxn - 2*n, "");
        for (int k = 0; k <= n; ++k) {
            printf("%3lu ", binom(n,k));
        }
        putchar('\n');
    }
    printf("-2 C 3: %lu\n", binom(-2,3));
    printf("2 C 3: %lu\n", binom(2,3));
    printf("2 C -1: %lu\n", binom(2,-1));
    printf("30 C 15: %20lu\n", binom(30,15)); // 155117520
    printf("60 C 30: %20lu\n", binom(60,30)); // 1.1826458156e17
    printf("70 C 27: %20lu\n", binom(70,27)); // 1.820855884e19
    // should barely fit in an unsigned long

    // This won't fit in a long
    printf("99 C 60: %20lu\n", binom(120,60)); // 5.498493658e27

    printf("2147483647 C 1: %lu\n", binom(2147483647,1));
    printf("2147483647 C 2: %lu\n", binom(2147483647,2));

    maxn = std::numeric_limits<int>::max();
    int mink = maxn;
    mpz_t c;
    mpz_init(c);

    for (int n = 13; n < maxn; ++n) {
        for (int k = 0; k <= n/2; ++k) {
            if (k >= mink) break;
            mpz_bin_uiui(c, n, k);
            if (mpz_fits_ulong_p(c)) {
                unsigned long d = binom(n, k);
                if (d != mpz_get_ui(c))
                    gmp_printf("Aaagh, %d C %d = %lu vs. %Z\n", n, k, d, c);
            } else {
                if (k < mink) {
                    mink = k;
                    printf("%d C %d\n", n, k);
                }
                break;
            }
        }
        if (n % (1 << 21) == 0) {
            putchar('.');
            fflush(stdout);
        }
    }
    return 0;
}

