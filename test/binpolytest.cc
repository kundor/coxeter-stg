#include "../binom.h"
#include <cstdio>
using std::vector;
using std::printf;

#define CHECK_EQ(ans, val) if (ans != val) \
    printf("Agh, %ld != %d on line %d!\n", ans, val, __LINE__);

int main() {
    vector<int> v {2, 0, 3};

    auto ans = binpoly(v, 1);
    // 2 (1 C 0) + 0 (1 C 1) + 3 (1 C 2) = 2 
    CHECK_EQ(ans, 2);

    ans = binpoly(v, 3);
    // 2 (3 C 0) + 0 (3 C 1) + 3 (3 C 2) = 2 + 3*3 = 11
    CHECK_EQ(ans, 11);

    ans = binpoly(v, 7);
    // 2 (7 C 0) + 0 (7 C 1) + 3 (7 C 2) = 2 + 3*21 = 65
    CHECK_EQ(ans, 65);

    ans = binpoly(v, 5, 1);
    // 2 (5 C 1) + 0 (5 C 2) + 3 (5 C 3) = 2*5 + 3*10 = 40
    CHECK_EQ(ans, 40);

    ans = binpoly(v, 4, 2);
    // 2 (4 C 2) + 0 (4 C 3) + 3 (4 C 4) = 2*6 + 3*1 = 15
    CHECK_EQ(ans, 15);

    ans = binpoly(v, 4, -2);
    // 2 (4 C -2) + 0 (4 C -1) + 3 (4 C 0) = 3
    CHECK_EQ(ans, 3);

    ans = binpoly(v, -1);
    // 2 (-1 C 0) + 0 (-1 C 1) + 3 (-1 C 2) = 0 here
    // (no negative binomial coefficients)
    CHECK_EQ(ans, 0);

    v = {3, 1, 2, -3, 5, -1};

    ans = binpoly(v, 1);
    // 3 (1 C 0) + 1 (1 C 1) + 2 (1 C 2) - 3 (1 C 3) + 5 (1 C 4) - 1 (1 C 5)
    //  = 3 + 1 
    CHECK_EQ(ans, 4);

    ans = binpoly(v, 3);
    // 3 (3 C 0) + 1 (3 C 1) + 2 (3 C 2) - 3 (3 C 3) + 5 (3 C 4) - 1 (3 C 5)
    //  = 3 + 1*3 + 2*3 - 3*1 = 9
    CHECK_EQ(ans, 9);

    ans = binpoly(v, -1);
    // 3 (-1 C 0) + 1 (-1 C 1) + 2 (-1 C 2) - 3 (-1 C 3) + 5 (-1 C 4) - 1 (-1 C 5)
    CHECK_EQ(ans, 0);

    ans = binpoly(v, 6);
    // 3 (6 C 0) + 1 (6 C 1) + 2 (6 C 2) - 3 (6 C 3) + 5 (6 C 4) - 1 (6 C 5)
    //  = 3 + 1*6 + 2*15 - 3*20 + 5*15 - 1*6 = 48
    CHECK_EQ(ans, 48);

    ans = binpoly(v, 2, 2);
    // 3 (2 C 2) + 1 (2 C 3) + 2 (2 C 4) - 3 (2 C 5) + 5 (2 C 6) - 1 (2 C 7)
    CHECK_EQ(ans, 3);

    ans = binpoly(v, 2, -2);
    // 3 (2 C -2) + 1 (2 C -1) + 2 (2 C 0) - 3 (2 C 1) + 5 (2 C 2) - 1 (2 C 3)
    //  = 0 + 0 + 2*1 - 3*2 + 5*1 - 0 = 1
    CHECK_EQ(ans, 1);

    return 0;
}
