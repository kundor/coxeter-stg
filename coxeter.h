#ifndef COXETER_DIAGRAM_H
#define COXETER_DIAGRAM_H
#include <boost/graph/adjacency_list.hpp>
#include <string>

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

typedef boost::adjacency_list <boost::vecS,
                               boost::vecS,
                               boost::undirectedS,
                               VertexProps,
                               EdgeOrder> CoxeterGraph;

typedef CoxeterGraph::vertices_size_type vsize_t; // it's std::size_t

class TeXout; //forward declaration; see TeXout.h

/* Return a linear Coxeter diagram, all of whose edges have order 3,
 * except the first edge is given by the parameter p.
 * p = 3 is A_n
 * p = 4 is B_n = C_n
 * p = 5 is H_n (finite when n is 2, 3, or 4)
 * p = 6 is G_n (finite when n is 2)
 * When n=2, this is I_2(p) (two nodes, where the single edge is labeled p)
 */
CoxeterGraph linear_coxeter(vsize_t n, unsigned p = 3);

/* Return a Coxeter diagram of type D_n */
CoxeterGraph coxeterD(vsize_t n);

/* Return a Coxeter diagram of type E_n, where n = 6, 7, or 8 */
CoxeterGraph coxeterE(vsize_t n);

/* Return a Coxeter diagram of type F_4 */
CoxeterGraph coxeterF4();

/* Given a character X among A, B, C, D, E, F, G, H, or I,
 * returns the Coxeter diagram of type X_n.
 * n is the number of nodes in every case except I,
 * where it is the label of the unique edge. */
CoxeterGraph coxeter_dispatch(char c, unsigned int n);

/* allringed: check if there is a ringed dot in every connected component
 * of a CoxeterGraph.  */
bool allringed(const CoxeterGraph& cg);

/* ringnodes: overloads to ring nodes in a CoxeterGraph: */ 
/*   Ring nodes corresponding to 1s in the string.
 *   The length of the string s should be at most num_vertices(cg). */
void ringnodes(CoxeterGraph& cg, const std::string& s);

/*   Ring nodes corresponding to set bits in the unsigned integer b.
 *   This only considers bits up to num_vertices(cg). */
void ringnodes(CoxeterGraph& cg, unsigned b);

/* Return a string containing a comma-separated list of the ringed nodes
 * in cg. */
std::string ringedlist(const CoxeterGraph& cg);

/* TikZ (LaTeX) representation of the graph.
 * These assume the presence of the tikzlibrary "quotes" and some definitions,
 * given by tikz_preamble */
TeXout& operator<<(TeXout& tex, const CoxeterGraph& cg);

#endif //COXETER_DIAGRAM_H
