/* Convert Coxeter diagrams with ringed nodes to a Hasse diagram of face orbits,
 * and from that to a symmetry type graph (a graph of flag orbits.)
 * This gives the number of orbits for t_{i,i+2} for A_n as n goes from 3 to 12.
 * It does not output the hasse diagrams or orbit graphs.
 */

#include "coxeter.h"
#include "poset.h"
#include <cstdio>
#include <cstdlib> // atoi
#include <vector>
#include <numeric> // partial_sum
#include <boost/algorithm/cxx11/any_of.hpp>

using std::printf;
using std::putchar;
using std::vector;
using boost::algorithm::any_of_equal;

template <typename T>
int vecsize(vector<T> v) {
    /* Return signed size */
    return v.size();
}

void gapring(int maxnode, vector<int> gaps) {
    gaps.insert(gaps.begin(), 0);
    std::partial_sum(gaps.begin(), gaps.end(), gaps.begin());
    if (maxnode <= gaps.back()) return;
    /* Header line */
    printf(" n");
    for (int i = 0; i < maxnode - gaps.back(); ++i) {
        for (int j = 0; j < vecsize(gaps); ++ j)
            if (i + gaps[j] < 10)
                putchar(' ');
        printf(" t_%d", i);
        for (int j = 1; j < vecsize(gaps); ++j)
            printf(",%d", i + gaps[j]);
    }
    putchar('\n');
    /* number of orbits */
    for (int numnode = gaps.back() + 1; numnode <= maxnode; ++numnode) {
        CoxeterGraph tcg = linear_coxeter(numnode);
        printf("%2d", numnode);
        for (int i = 0; i < numnode - gaps.back(); ++i) {
            for (int v = 0; v < numnode; ++v) {
                tcg[v].ringed = any_of_equal(gaps, v - i);
            }
            FaceOrbitPoset hasse{tcg};
            printf("%*d", 2 + 3*vecsize(gaps), hasse.head->numpaths());
        }
        putchar('\n');
    }
}

int main(int argc, char* argv[]) {
    const int maxnode = argc > 1 ? std::atoi(argv[1]) : 12;
    if (maxnode < 3 || maxnode > 256) {
        printf("First argument must be positive integer "
               "(maximum number of nodes)\n");
        return 1;
    }
    gapring(maxnode, {});
    putchar('\n');
    gapring(maxnode, {1});
    putchar('\n');
    gapring(maxnode, {2});
    putchar('\n');
    gapring(maxnode, {3});
    putchar('\n');
    gapring(maxnode, {1, 1});
    return 0;
}
