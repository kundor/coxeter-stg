/* Convert Coxeter diagrams with ringed nodes to a Hasse diagram of face orbits,
 * and from that to a symmetry type graph (a graph of flag orbits.)
 * This gives the number of orbits for t_{i,i+2} for A_n as n goes from 3 to 12.
 * It does not output the hasse diagrams or orbit graphs.
 */

#include "coxeter.h"
#include "poset.h"
#include <cstdio>

using std::printf;
using std::putchar;

void onering(unsigned maxnode) {
    /* Header line */
    printf(" n");
    for (unsigned i = 0; i < maxnode; ++i) {
        if (i < 10)
            putchar(' ');
        printf(" t_%d", i);
    }
    putchar('\n');
    /* number of orbits */
    for (unsigned numnode = 1; numnode <= maxnode; ++numnode) {
        CoxeterGraph tcg = coxeterA(numnode);
        printf("%2d", numnode);
        for (unsigned i = 0; i < numnode; ++i) {
            for (unsigned v = 0; v < numnode; ++v) {
                if (v == i)
                    tcg[v].ringed = true;
                else
                    tcg[v].ringed = false;
            }
            FaceOrbitPoset hasse{tcg};
            printf("%5d", hasse.head->numpaths());
        }
        putchar('\n');
    }
}

void tworing(unsigned maxnode, unsigned gap) {
    if (maxnode < gap) return;
    /* Header line */
    printf(" n");
    for (unsigned i = 0; i < maxnode - gap; ++i) {
        if (i < 10 - gap)
            putchar(' ');
        if (i < 10)
            putchar(' ');
        printf(" t_%d,%d", i, i + gap);
    }
    putchar('\n');
    /* number of orbits */
    for (unsigned numnode = gap + 1; numnode <= maxnode; ++numnode) {
        CoxeterGraph tcg = coxeterA(numnode);
        printf("%2d", numnode);
        for (unsigned i = 0; i < numnode - gap; ++i) {
            for (unsigned v = 0; v < numnode; ++v) {
                if (v == i || v == i + gap)
                    tcg[v].ringed = true;
                else
                    tcg[v].ringed = false;
            }
            FaceOrbitPoset hasse{tcg};
            printf("%8d", hasse.head->numpaths());
        }
        putchar('\n');
    }
}

void threering(unsigned maxnode, unsigned gap=1) {
    if (maxnode < 2*gap) return;
    /* Header line */
    printf(" n");
    for (unsigned i = 0; i < maxnode - 2*gap; ++i) {
        if (i < 10 - 2*gap)
            putchar(' ');
        if (i < 10 - gap)
            putchar(' ');
        if (i < 10)
            putchar(' ');
        printf(" t_%d,%d,%d", i, i + gap, i + 2*gap);
    }
    putchar('\n');
    /* number of orbits */
    for (unsigned numnode = 2*gap + 1; numnode <= maxnode; ++numnode) {
        CoxeterGraph tcg = coxeterA(numnode);
        printf("%2d", numnode);
        for (unsigned i = 0; i < numnode - 2*gap; ++i) {
            for (unsigned v = 0; v < numnode; ++v) {
                if (v == i || v == i + gap || v == i + 2*gap)
                    tcg[v].ringed = true;
                else
                    tcg[v].ringed = false;
            }
            FaceOrbitPoset hasse{tcg};
            printf("%11d", hasse.head->numpaths());
        }
        putchar('\n');
    }
}

int main() {
    const int maxnode = 12;
    onering(maxnode);
    putchar('\n');
    tworing(maxnode, 1);
    putchar('\n');
    tworing(maxnode, 2);
    putchar('\n');
    tworing(maxnode, 3);
    putchar('\n');
    threering(maxnode);
    return 0;
}
