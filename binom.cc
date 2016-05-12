#ifdef BINOM_ONLY
#  include <cstdint>
#else
#  include "binom.h"
#  include <boost/algorithm/cxx11/all_of.hpp> // all_of_equal
#  include <numeric> // adjacent_difference
#  include <iterator> // distance

using std::vector;
using std::adjacent_difference;
using boost::algorithm::all_of_equal;
#endif

#ifdef BINOM_CHECK
#  include <stdexcept>
#endif

static uint64_t gcd(uint64_t m, uint64_t n) {
   while (n != 0) {
      m %= n;
      if (m == 0) return n;
      n %= m;
    }
    return m;
}

#ifdef BINOM_CHECK
static bool bcfits64(int n, int k) {
    return (n <      68 || k < 31) &&
           (n <      69 || k < 29) &&
           (n <      70 || k < 28) &&
           (n <      71 || k < 27) &&
           (n <      72 || k < 26) &&
           (n <      73 || k < 25) &&
           (n <      75 || k < 24) &&
           (n <      77 || k < 23) &&
           (n <      79 || k < 22) &&
           (n <      83 || k < 21) &&
           (n <      87 || k < 20) &&
           (n <      92 || k < 19) &&
           (n <      98 || k < 18) &&
           (n <     106 || k < 17) &&
           (n <     117 || k < 16) &&
           (n <     131 || k < 15) &&
           (n <     151 || k < 14) &&
           (n <     178 || k < 13) &&
           (n <     219 || k < 12) &&
           (n <     283 || k < 11) &&
           (n <     387 || k < 10) &&
           (n <     578 || k <  9) &&
           (n <     968 || k <  8) &&
           (n <    1914 || k <  7) &&
           (n <    4869 || k <  6) &&
           (n <   18581 || k <  5) &&
           (n <  145057 || k <  4) &&
           (n < 4801281 || k <  3);
}
#endif

uint64_t binom(int n, int k) {
    if (k < 0 || n < k) // also covers n < 0
        return 0;
    if (k > n/2)
        k = n - k;
    if (k == 0)
        return 1;
#ifdef BINOM_CHECK
    if (!bcfits64(n, k))
        throw std::overflow_error("Result will not fit in 64 bits.");
#endif
    uint64_t result = 1;
    const unsigned kk = k; // This is just to get rid of Wsign-compare
    for (uint64_t i = 1; i <= kk; ++i) {
        uint64_t mul = n - k + i;
        uint64_t d = gcd(result, i);
        result /= d;
        mul /= (i/d);
        result *= mul;
    }
    return result;
}

#ifndef BINOM_ONLY

long binpoly(const vector<int>& coef, int n, int offset) {
    long ans = 0;
    for (int i = 0; static_cast<unsigned>(i) < coef.size(); ++i) {
        ans += coef[i] * binom(n, offset + i);
    }
    return ans;
}

template <typename It>
static bool is_constant(It b, It e) {
    if (std::distance(b, e) <= 1) return true;
    return all_of_equal(b + 1, e, *b);
}

vector<int> seqsolver(vector<int> v, int start) {
    auto d = v.begin();
    while(!is_constant(d, v.end())) {
        adjacent_difference(d, v.end(), d);
        ++d;
    }
    if (std::distance(d, v.end()) < 3)
        return {}; // not enough terms 
    const int n = std::distance(v.begin(), d);
    vector<int> coef(n + 1);
    for (int i = n; i >= 0; --i) {
        coef[i] = v[i] - binpoly(coef, start, -i);
    }
    return coef;
}

#endif //BINOM_ONLY
