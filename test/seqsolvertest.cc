#include "../seqsolver.cc"
#include <iostream>

using std::cout;

std::ostream& operator<<(std::ostream& os, const vector<int>& v) {
     std::copy(v.begin(), v.end(), std::ostream_iterator<int>(os, " "));
     return os;
}


#define CHECK_EQ_VEC(ans, val) if (outifdiff(ans, val)) \
    cout << " on line " << __LINE__ << '\n';

bool outifdiff(vector<int> ans, vector<int> val) {
    if (ans != val) {
        cout << "Agh, " << ans << " != " << val;
        return true;
    }
    return false;
}

int main() {
    vector<int> v {56, 231, 672, 1596, 3312, 6237, 10912, 18018};

    auto ans = seqsolver(v, 6);
    CHECK_EQ_VEC(*ans, (vector<int>{0, 0, 0, 1, -4, 16}));

    ans = seqsolver({6, 6, 6, 6, 6});
    CHECK_EQ_VEC(*ans, (vector<int>{6}));

    ans = seqsolver({0, 1, 4, 10, 20, 35}, 2);
    CHECK_EQ_VEC(*ans, (vector<int>{0,0,0,1}));

    ans = seqsolver({1});
    if (ans) {
        cout << "{1}: " << *ans << '\n';
    } else {
        cout << "Could not solve {1}\n";
    }

    ans = seqsolver({1,1});
    if (ans) {
        cout << "{1,1}: " << *ans << '\n';
    } else {
        cout << "Could not solve {1,1}\n";
    }

    ans = seqsolver({1,1,1});
    if (ans) {
        cout << "{1,1,1}: " << *ans << '\n';
    } else {
        cout << "Could not solve {1,1,1}\n";
    }

    ans = seqsolver({1,2,3,4});
    if (ans) {
        cout << "{1,2,3,4}: " << *ans << '\n';
    } else {
        cout << "Could not solve {1,2,3,4}\n";
    }

    ans = seqsolver({4,3,2,1});
    if (ans) {
        cout << "{4,3,2,1}: " << *ans << '\n';
    } else {
        cout << "Could not solve {4,3,2,1}\n";
    }





    return 0;
}
