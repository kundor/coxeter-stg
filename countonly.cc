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

void gapring(unsigned maxnode, vector<unsigned> gaps) {
    gaps.insert(gaps.begin(), 0);
    std::partial_sum(gaps.begin(), gaps.end(), gaps.begin());
    if (maxnode <= gaps.back()) return;
    /* Header line */
    printf(" n");
    for (unsigned i = 0; i < maxnode - gaps.back(); ++i) {
        for (unsigned j = 0; j < gaps.size(); ++ j)
            if (i + gaps[j] < 10)
                putchar(' ');
        printf(" t_%d", i);
        for (unsigned j = 1; j < gaps.size(); ++j)
            printf(",%d", i + gaps[j]);
    }
    putchar('\n');
    /* number of orbits */
    for (unsigned numnode = gaps.back() + 1; numnode <= maxnode; ++numnode) {
        CoxeterGraph tcg = linear_coxeter(numnode);
        printf("%2d", numnode);
        for (unsigned i = 0; i < numnode - gaps.back(); ++i) {
            for (unsigned v = 0; v < numnode; ++v) {
                tcg[v].ringed = any_of_equal(gaps, v - i);
            }
            FaceOrbitPoset hasse{tcg};
            printf("%*d", 2 + 3*gaps.size(), hasse.head->numpaths());
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
