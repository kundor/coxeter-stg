/* Convert Coxeter diagram with ringed nodes to a Hasse diagram of face orbits,
 * and from that to a symmetry type graph (a graph of flag orbits.)
 */

#include <fstream>
#include <unistd.h> // execlp
#include "TeXout.h"
#include "coxeter.h"
#include "poset.h"

int main() {
    std::ios_base::sync_with_stdio(false);
    TeXout tex;
    tex.usetikzlibrary("positioning");
    size_t numnode = 4; // less than 64
    CoxeterGraph tcg = coxeterA(numnode);
    for (unsigned b = 1u; b < (1u << numnode); ++b) {
        for (unsigned v = 0; v < numnode; ++v) {
            if (b & (1u << v)) // if bit v is set
                tcg[v].ringed = true;
            else
                tcg[v].ringed = false;
        }
        FaceOrbitPoset hasse{tcg};
        auto orbgraph = makeOrbit(hasse);

        tex << "\\begin{tikzpicture}\n"
               "\\node (N) {" << env_wrap{"tikzpicture"} << hasse << "};\n"
               "\\node (O) [below=of N] {" << env_wrap{"tikzpicture"} << orbgraph << "};\n"
               "\\node[left=of O] {" << hasse.head->numpaths() << " flag orbits:};\n"
               "\\end{tikzpicture}\n";
    }

    std::ofstream file("output.tex");
    file << tex;
    file.close();

    execlp("lualatex", "lualatex", "output", (char*)NULL);
    // this should never return; our process is replaced.
    // if it does return, there is an error.
    return 1;
}
