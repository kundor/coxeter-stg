#include "binom.h"
#include <boost/algorithm/cxx11/all_of.hpp> // all_of_equal
#include <numeric> // adjacent_difference
#include <iterator> // distance
#include "optional.h"

using std::vector;
using std::adjacent_difference;

template <typename It>
bool is_constant(It b, It e) {
    if (std::distance(b, e) <= 1) return true;
    return boost::algorithm::all_of_equal(b + 1, e, *b);
    /* Alternative possibilities:
     *  return std::all_of(b + 1, e, [val=*b](auto n){ return n == val; });
     * or with
     *  std::bind(std::equal_to<int>(), std::placeholders::_1, *b)
     * instead of the lambda (needs <functional>)
     *
     *  auto mnmx = std::minmax_element(b, e);
     *  return *mnmx.first == *mnmx.second;
     * Presumably all_of is faster because it can short-circuit */
}

optional<vector<int>> seqsolver(vector<int> v, int start = 0) {
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
