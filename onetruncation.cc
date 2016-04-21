/* Convert Coxeter diagram with ringed nodes to a Hasse diagram of face orbits,
 * and from that to a symmetry type graph (a graph of flag orbits.)
 * This gets t_{0,1,2} for A_n as n goes from 3 to 10.
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
    for (size_t numnode = 3; numnode < 11; ++numnode) {
        CoxeterGraph tcg = coxeterA(numnode);
        tcg[0].ringed = true;
        tcg[1].ringed = true;
        tcg[2].ringed = true;
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
