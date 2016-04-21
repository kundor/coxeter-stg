#ifndef COXETER_DIAGRAM_H
#define COXETER_DIAGRAM_H
#include <boost/graph/adjacency_list.hpp>

/* Coxeter diagram: an undirected graph, with nodes either ringed or not (bool),
 * and edges labeled by an integer, or ∞ (represented by 0 here).
 * (Consider doubles for edge labels, to allow fractional labels and literal ∞)
 */
struct VertexProps {
    bool ringed;
    int x_coord;
    int y_coord;
};

struct EdgeOrder {
    unsigned int order;
};

typedef boost::adjacency_list <boost::vecS, boost::vecS, boost::undirectedS, VertexProps, EdgeOrder> CoxeterGraph;
typedef CoxeterGraph::vertices_size_type vsize_t; // it's std::size_t

class TeXout; //forward declaration; see TeXout.h

/* Return a Coxeter diagram of type A_n */
CoxeterGraph coxeterA(vsize_t n);

/* Return a Coxeter diagram of type B_n = C_n */
CoxeterGraph coxeterBC(vsize_t n);

/* Return a Coxeter diagram of type D_n */
CoxeterGraph coxeterD(vsize_t n);

/* Return a Coxeter diagram of type E_n, where n = 6, 7, or 8 */
CoxeterGraph coxeterE(vsize_t n);

/* Return a Coxeter diagram of type F_4 */
CoxeterGraph coxeterF4();

/* Return a Coxeter diagram of type H_n, where n = 2, 3, or 4 */
CoxeterGraph coxeterH(vsize_t n);

/* Return a Coxeter diagram with two nodes, where the single edge
 * is labeled p, sometimes called I_p; when p = 3, this is A_2;
 * when p = 4, this is B_2; when p = 5, this is H_2; when p = 6, this is G_2 */
CoxeterGraph coxeterI(unsigned int p);

/* allringed: check if there is a ringed dot in every connected component
 * of a CoxeterGraph.  */
bool allringed(const CoxeterGraph& cg);

/* TikZ (LaTeX) representation of the graph.
 * These assume the presence of the tikzlibrary "quotes" and some definitions,
 * given by tikz_preamble */
TeXout& operator<<(TeXout& tex, const CoxeterGraph& cg);

#endif //COXETER_DIAGRAM_H
