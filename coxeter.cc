#include "coxeter.h"
#include "TeXout.h"
#include <boost/graph/connected_components.hpp>
#include <vector>
#include <algorithm>
/* Coxeter diagram: an undirected graph, with nodes either ringed or not (bool),
 * and edges labeled by an integer, or ∞ (represented by 0 here).
 * (Consider doubles for edge labels, to allow fractional labels and literal ∞)
 */

using boost::num_vertices;
using boost::add_edge;
typedef CoxeterGraph::vertex_descriptor vdesc; // it's std::size_t

namespace { // functions available only in this file (static)
    bool all(const std::vector<bool>& vb) {
        /* Return true iff all elements of vb are true. */
        return std::all_of(vb.begin(), vb.end(),
                  [](bool b) { return b; });
    }

    void initialize(CoxeterGraph& cg) {
        for (auto vits = boost::vertices(cg); vits.first != vits.second; ++vits.first) {
            cg[*vits.first].ringed = false;
            cg[*vits.first].x_coord = 0;
            cg[*vits.first].y_coord = 0;
        }
    }
}

/*
 * allringed: check if there is a ringed dot in every connected component
 * of a CoxeterGraph.
 */
bool allringed(const CoxeterGraph& cg) {
    const vsize_t nv = num_vertices(cg);
    std::vector<int> component(nv);
    const int nc = boost::connected_components(cg, &component[0]);
    std::vector<bool> ringincomp(nc);
    for (vdesc v = 0; v < nv; ++v) {
        if (cg[v].ringed) {
            ringincomp[component[v]] = true;
        }
    }
    return all(ringincomp);
}


/* Return a Coxeter diagram of type A_n */
CoxeterGraph coxeterA(vsize_t n) {
    CoxeterGraph cg{n};
    initialize(cg);
    if (n < 1) return cg; // do not make an 18-pentillion node graph
    for (vdesc i = 0; i < n - 1; ++i) {
        add_edge(i, i+1, {3u}, cg);
    }
    for (vdesc i = 0; i < n; ++i) {
        cg[i].x_coord = i;
    }
    return cg;
}

/* Return a Coxeter diagram of type B_n = C_n */
CoxeterGraph coxeterBC(vsize_t n) {
    CoxeterGraph cg{n};
    initialize(cg);
    for (vdesc i = 0; i < n; ++i) {
        cg[i].x_coord = i;
    }
    if (n < 2) return cg;
    add_edge(0u, 1u, {4u}, cg);
    for (vdesc i = 1; i < n - 1; ++i) {
        add_edge(i, i+1, {3u}, cg);
    }
    return cg;
}

/* Return a Coxeter diagram of type D_n */
CoxeterGraph coxeterD(vsize_t n) {
    CoxeterGraph cg{n};
    initialize(cg);
    if (n < 2) return cg;
    cg[n-1].x_coord = n - 2;
    cg[n-1].y_coord = 1;
    cg[n-2].x_coord = n - 2;
    cg[n-2].y_coord = -1;
    if (n < 3) return cg;
    /* D_2 is a pair of unconnected nodes, A_1 × A_1 */
    
    for (vdesc i = 0; i < n - 2; ++i) {
        add_edge(i, i+1, {3u}, cg);
        cg[i].x_coord = i;
    }
    add_edge(n-3, n-1, {3u}, cg);
    return cg;
}

/* Return a Coxeter diagram of type E_n, where n = 6, 7, or 8.
 *  (when n = 5, same as D_5.)
 *  This continues the pattern to build a graph if n > 8, but it
 *  doesn't correspond to a finite group. */
CoxeterGraph coxeterE(vsize_t n) {
    CoxeterGraph cg{n};
    initialize(cg);
    if (n < 2) return cg; // do not make 18 pentillion node graph
    for (vdesc i = 0; i < n - 2; ++i) {
        add_edge(i, i+1, {3u}, cg);
    }
    for (vdesc i = 0; i < (n < 4 ? n : n - 1); ++i) {
        cg[i].x_coord = i;
    }
    if (n < 4) return cg;
    add_edge(2u, n-1, {3u}, cg);
    cg[n-1].x_coord = 2;
    cg[n-1].y_coord = 1;
    return cg;
}

/* Return a Coxeter diagram of type F_4 */
CoxeterGraph coxeterF4() {
    CoxeterGraph cg{4};
    initialize(cg);
    add_edge(0u, 1u, {3u}, cg);
    add_edge(1u, 2u, {4u}, cg);
    add_edge(2u, 3u, {3u}, cg);
    for (vdesc i = 0; i < 4; ++i) {
        cg[i].x_coord = i;
    }
    return cg;
}


/* Return a Coxeter diagram of type H_n, where n = 2, 3, or 4 */
CoxeterGraph coxeterH(vsize_t n) {
    CoxeterGraph cg{n};
    initialize(cg);
    if (n < 2) return cg;
    add_edge(0u, 1u, {5u}, cg);
    for (vdesc i = 1; i < n - 1; ++i) {
        add_edge(i, i+1, {3u}, cg);
    }
    for (vdesc i = 0; i < n; ++i) {
        cg[i].x_coord = i;
    }
    return cg;
}

/* Return a Coxeter diagram with two nodes, where the single edge
 * is labeled p, sometimes called I_2(p); when p = 3, this is A_2;
 * when p = 4, this is B_2; when p = 5, this is H_2; when p = 6, this is G_2 */
CoxeterGraph coxeterI(unsigned int p) {
    CoxeterGraph cg{2};
    initialize(cg);
    add_edge(0u, 1u, {p}, cg);
    cg[1].x_coord = 1; //default 0 for the rest
    return cg;
}

TeXout& operator<<(TeXout& tex, const CoxeterGraph& cg) {
    static bool inited {false};

    if (!inited) {
        tex.usepackage("tikz");
        tex.usetikzlibrary("quotes");
        tex.addtopreamble("\\tikzset{\n"
            "  dot/.style={fill,circle,inner sep=2pt,outer sep=0pt},\n"
            "  dit/.style={fill,circle,inner sep=1.4pt,outer sep=0pt},\n"
            "  ring/.style={draw,circle,inner sep=2.2pt}\n"
            "}\n");
        inited = true;
    }

    for (unsigned v = 0; v < num_vertices(cg); ++v) {
        tex << "\\node["
            << (cg[v].ringed ? "dit" : "dot")
            << "] (v" << v << ") at ("
            << cg[v].x_coord << ", "
            << cg[v].y_coord << ") {};\n";
        if (cg[v].ringed) {
            tex << "\\node[ring] at ("
                << cg[v].x_coord << ", "
                << cg[v].y_coord << ") {};\n";
        }
    }
    auto edgits = boost::edges(cg);
    if (edgits.first == edgits.second)
        return tex;
    tex << "\\draw";
    for (auto eit = edgits.first; eit != edgits.second; ++eit) {
        tex << "  (v" << boost::source(*eit, cg) << ") to";
        if (cg[*eit].order == 0)
            tex << "[\"\\infty\"]";
        else if (cg[*eit].order != 3)
            tex << "[\"" << cg[*eit].order << "\"]";
        tex << " (v" << boost::target(*eit, cg) << ")";
    }
    return tex << ";\n";
}
