/* Convert Coxeter diagrams with ringed nodes to a Hasse diagram of face orbits,
 * and from that to a symmetry type graph (a graph of flag orbits.)
 * This gives the number of orbits for t_{i,i+2} for A_n as n goes from 3 to 12.
 * It does not output the hasse diagrams or orbit graphs.
 */

#include "coxeter.h"
#include "poset.h"
#include <cstdio>
#include <cstdlib> // atoi

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
        CoxeterGraph tcg = linear_coxeter(numnode);
        printf("%2d", numnode);
        for (unsigned i = 0; i < numnode; ++i) {
            if (i) tcg[i-1].ringed = false;
            tcg[i].ringed = true;
            FaceOrbitPoset hasse{tcg};
            printf("%5d", hasse.head->numpaths());
        }
        putchar('\n');
    }
}

void tworing(unsigned maxnode, unsigned gap) {
    if (maxnode <= gap) return;
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
        CoxeterGraph tcg = linear_coxeter(numnode);
        printf("%2d", numnode);
        for (unsigned i = 0; i < numnode - gap; ++i) {
            for (unsigned v = 0; v < numnode; ++v) {
                tcg[v].ringed = (v == i || v == i + gap);
            }
            FaceOrbitPoset hasse{tcg};
            printf("%8d", hasse.head->numpaths());
        }
        putchar('\n');
    }
}

void threering(unsigned maxnode, unsigned gap1=1, unsigned gap2=1) {
    if (maxnode <= gap1 + gap2) return;
    /* Header line */
    printf(" n");
    for (unsigned i = 0; i < maxnode - gap1 - gap2; ++i) {
        if (i < 10 - gap1 - gap2)
            putchar(' ');
        if (i < 10 - gap1)
            putchar(' ');
        if (i < 10)
            putchar(' ');
        printf(" t_%d,%d,%d", i, i + gap1, i + gap1 + gap2);
    }
    putchar('\n');
    /* number of orbits */
    for (unsigned numnode = gap1 + gap2 + 1; numnode <= maxnode; ++numnode) {
        CoxeterGraph tcg = linear_coxeter(numnode);
        printf("%2d", numnode);
        for (unsigned i = 0; i < numnode - gap1 - gap2; ++i) {
            for (unsigned v = 0; v < numnode; ++v) {
                tcg[v].ringed = (v == i || v == i + gap1 || v == i + gap1 + gap2);
            }
            FaceOrbitPoset hasse{tcg};
            printf("%11d", hasse.head->numpaths());
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
