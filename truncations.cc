/* Convert Coxeter diagram with ringed nodes to a Hasse diagram of face orbits,
 * and from that to a symmetry type graph (a graph of flag orbits.)
 * This gets t_{0,1,2} for A_n as n goes from 3 to 10.
 */

#include "TeXout.h"
#include "coxeter.h"
#include "poset.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <boost/program_options.hpp>
#include <unistd.h> // execlp

namespace po = boost::program_options;
using std::string;
using boost::num_vertices;

namespace { // this-file-only (internal linkage)
    /* Ring all the nodes in cg, starting from the 0th, corresponding to
     * '1's in the string s (which is presumed to consist of 0s and 1s.) */
    void ringstr(CoxeterGraph& cg, const string& s) {
        for (unsigned i = 0; i < num_vertices(cg); ++i)
            cg[i].ringed = false;
        for (unsigned i = 0; i < s.size(); ++i)
            if (s[i] == '1')
                cg[i].ringed = true;
    }

    /* Ring all the nodes in cg corresponding to set bits
     * in the unsigned integer b */
    void ringbits(CoxeterGraph& cg, unsigned b) {
        for (unsigned v = 0; v < num_vertices(cg); ++v) {
            if (b & (1u << v)) // if bit v is set
                cg[v].ringed = true;
            else
                cg[v].ringed = false;
        }
    }

    /* Return a string containing a comma-separated list of the ringed nodes
     * in cg. */
    string ringedlist(const CoxeterGraph& cg) {
        string s;
        bool first = true;
        for (unsigned v = 0; v < num_vertices(cg); ++v) {
            if (cg[v].ringed) {
                if (first) first = false;
                else s += ',';
                s += std::to_string(v);
            }
        }
        return s;
    }

    void texgraphs(TeXout& tex, const FaceOrbitPoset& hasse) {
        auto orbgraph = makeOrbit(hasse);
        tex << "\\begin{tikzpicture}\n"
               "\\node (N) {" << env_wrap{"tikzpicture"} << hasse << "};\n"
               "\\node (O) [below=of N] {" << env_wrap{"tikzpicture"} << orbgraph << "};\n"
               "\\node[left=of O] {" << hasse.head->numpaths() << " flag orbits:};\n"
               "\\end{tikzpicture}\n";
    }

    void output(const po::variables_map& vm, TeXout& tex, const CoxeterGraph& cg) {
        FaceOrbitPoset hasse{cg};
        if (vm.count("tex") || vm.count("pdf"))
            texgraphs(tex, hasse);
        if (vm.count("count"))
            std::cout << "t_{" << ringedlist(cg) << "}("
                      << num_vertices(cg) << ")\t"
                      << hasse.head->numpaths() << '\n';
        // Ideally, this would factor in the maximum width of the ringed list
        // and align the program's output appropriately
    }
}

int main(int argc, char* argv[]) {
    std::ios_base::sync_with_stdio(false);

    int maxnodes, numnode{0};
    string texfile, diagram, trunc;
    bool usage;

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h",     po::bool_switch(&usage),
           "Produce help message")
        ("diagram,d",  po::value<string>(&diagram),
           "Coxeter diagram to start with, e.g. A4, D5, E6. "
           "If -t is not specified, perform every possible truncation.")
        ("number,n",   po::value<int>(&numnode)->value_name("<n>"),
           "Number of nodes; equivalent to -d A<n>")
        ("truncate,t", po::value<string>(&trunc)->value_name("<pattern>"),
           "Truncation pattern: 0 for unringed, 1 for ringed, to apply "
           "starting from node 0. If -d or -n are not specified, "
           "applied to A_n as n ranges from the length of the pattern "
           "up to maxnodes.")
        ("maxnodes,m", po::value<int>(&maxnodes)->default_value(12),
           "Maximum number of nodes to consider (when -d or -n are not given)")
        ("count,c",
           "Print the number of flag orbits to the console")
        ("tex,x",      po::value<string>(&texfile)->implicit_value("output.tex"),
           "Write LaTeX output to the given file")
        ("pdf,p",
           "Convert TeX output to PDF. Implies -x.");

    /* Process command line */
    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
        /* Option values are stored in vm */
    } catch(std::exception& e) {
        /* Report command-line problems without "abort trap" ugliness */
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    if (vm.count("pdf") && texfile.empty())
        texfile = "output.tex";
    
    if (vm.count("diagram")) {
        if (vm.count("number")) {
            std::cerr << "It is an error to supply both -d and -n options.\n";
            usage = true;
        }
        size_t pos = 0;
        try {
            numnode = std::stoi(diagram.substr(1), &pos);
        } catch(std::exception& e) {
            numnode = 0;
        }
        if (diagram[0] < 'A' ||
                diagram[0] > 'I' ||
                pos != diagram.size() - 1 ||
                numnode <= 0) {
            std::cerr << "Invalid Coxeter diagram name!\n"
                         "There must be a character (one of ABCDEFGHI) "
                         "followed by a positive number (the number of nodes), "
                         "with no spaces.\n"; 
            usage = true;
        }
    } else if (vm.count("number")) {
        if (numnode <= 0) {
            std::cerr << "Number of nodes must be positive\n";
            usage = true;
        }
    } else if (vm.count("truncate")) {
        if (trunc.empty() ||
                !std::all_of(trunc.begin(), trunc.end(),
                    [](char c){ return c == '0' || c == '1'; })) {
            std::cerr << "<pattern> must consist of 0s and 1s.\n";
            usage = true;
        }
    } else {
        std::cerr << "At least one of -d, -n, or -t must be supplied.\n";
        usage = true;
    }

    if (maxnodes <= static_cast<int>(trunc.size())) {
        std::cerr << "Value of maxnodes (-m) must be positive and at least "
                     "as large as the length of <pattern>, if given.\n";
        usage = true;
    }

    if (!vm.count("count") && !vm.count("tex") && !vm.count("pdf")) {
        std::cerr << "At least one of -c, -x, or -p must be specified, "
                     "or there is no output.\n";
        usage = true;
    }

    if (usage) {
        std::cerr << desc << "\n";
        return 1;
    }
    /* Done processing options */

    TeXout tex;
    tex.usetikzlibrary("positioning");
    CoxeterGraph tcg;

    if (numnode == 0) { // no diagram specified
        for (numnode = trunc.size(); numnode <= maxnodes; ++numnode) {
            tcg = linear_coxeter(numnode);
            ringstr(tcg, trunc);
            output(vm, tex, tcg);
        }
    } else { // specific diagram given
        if (vm.count("diagram"))
            tcg = coxeter_dispatch(diagram[0], numnode);
        else if (vm.count("number"))
            tcg = linear_coxeter(numnode);

        if (!trunc.empty()) { // do one truncation of one diagram
            ringstr(tcg, trunc);
            output(vm, tex, tcg);
        } else { // Do all truncations
            for (unsigned b = 1u; b < (1u << numnode); ++b) {
                ringbits(tcg, b);
                output(vm, tex, tcg);
            }
        }
    }

    if (!texfile.empty()) {
        std::ofstream file(texfile);
        file << tex;
    }

    if (vm.count("pdf")) {
        execlp("lualatex", "lualatex", texfile.c_str(), (char*)NULL);
        // this should never return; our process is replaced.
        // if it does return, there is an error.
        return 2;
    }

    return 0;
}
