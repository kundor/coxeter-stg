#ifndef NAM_POSET_H
#define NAM_POSET_H

#include <vector>
#include <set>
#include <array>
#include <boost/dynamic_bitset.hpp>
#include "coxeter.h" // includes <boost/graph/adjacency_list.hpp> and forward-declares TeXout

typedef boost::dynamic_bitset<> bitset;

/*************
 * PosetNode *
 *************/

struct PosetNode {
    bitset bs;
    /* The bitset keeps track of which dots of the original CoxeterGraph
     * (the top-rank node in the poset)
     * are present in this subgraph. Really, this should be all the information
     * needed, but we keep the CoxeterGraph object to use for boost::graph
     * algorithms like connected_components */
    CoxeterGraph cg;
    mutable std::vector<const PosetNode*> parents;
    mutable std::vector<const PosetNode*> children;
    /* We keep the PosetNodes in std::sets to find duplicate subgraphs.
     * That forces them to be const. But we want to change the parents and
     * children, which doesn't affect the bitset, which is used for the ordering.
     * So they're marked mutable. */
 
    int numpaths() const;
    std::vector<std::vector<const PosetNode*>> chains() const;
    
    double x_avg() const;
    std::array<double,6> x_tuple() const;

    bool operator< (const PosetNode& other) const {
        return bs < other.bs;
    }
};

/******************
 * FaceOrbitPoset *
 ******************/

struct FaceOrbitPoset {
    std::vector<std::set<PosetNode>> nodes;
    const PosetNode* head;

    FaceOrbitPoset(const CoxeterGraph& cg);
    void genchildren();
    void to_tikz(TeXout& tex) const;
};

inline TeXout& operator<<(TeXout& tex, const FaceOrbitPoset& fop) {
    fop.to_tikz(tex);
    return tex;
}

/*** Orbit Graphs ***/
/* These should logically be in their own header, but there are only four
 * lines here */

struct EdgeRank {
    int rank;
};

typedef boost::adjacency_list<boost::vecS, //container for edges at each vertex (vector)
                              boost::vecS, //container for the vertices (vector)
                              boost::undirectedS,
                              boost::no_property, //no vertex properties
                              EdgeRank> // edges have rank
                              OrbitGraph;

OrbitGraph makeOrbit(const FaceOrbitPoset& hasse);
TeXout& operator<<(TeXout& tex, const OrbitGraph& og);

#endif // NAM_POSET_H
