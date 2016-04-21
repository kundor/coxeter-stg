#include "poset.h"
#include <algorithm>
#include <numeric> // accumulate
#include <cmath> // log2
#include "optional.h" // get std::optional or equivalent, as ::optional
#include "TeXout.h"

using std::vector;
using std::max;
using boost::num_vertices;
typedef CoxeterGraph::vertex_descriptor Vertex; // it's std::size_t


/*********************
 * Utility functions *
 *********************/
namespace {
    /* If containers a and b differ at exactly one index, return that index. */
    template <typename V>
    optional<int> diffinone(V a, V b) {
        if (a.size() != b.size())
            return {};
        bool differ = false;
        int m = -1;
        for (size_t i = 0; i < a.size(); ++i) {
            if (a[i] != b[i]) {
                if (differ) return {};
                differ = true;
                m = i;
            }
        }
        if (differ) return m;
        return {};
    }

    /* Find the range of a (numeric) vertex property on a graph */
    template <typename PropMap>
    int proprange(const PropMap& p) {
        auto vits = boost::vertices(*p.m_g); 
        auto mnmx = std::minmax_element(vits.first, vits.second,
                [&p](Vertex a, Vertex b) {
                    return get(p, a) < get(p, b);
                });
        return get(p, *mnmx.second) - get(p, *mnmx.first);
    }

    const PosetNode& inserter(std::set<PosetNode>& s, bitset&& b, const CoxeterGraph& cg) {
        return *(s.insert({b, cg, {}, {} }).first);
    }
}

/*************
 * PosetNode *
 *************/
 
int PosetNode::numpaths() const {
    // The number of paths leading down from this node
    if (children.empty())
        return 1;
    int sum{0};
    for (auto kid : children) {
        sum += kid->numpaths();
    }
    return sum;
}

vector<vector<const PosetNode*>> PosetNode::chains() const {
    if (children.empty())
        return { { this } };
    vector<vector<const PosetNode*>> chains;
    for (auto kid : children) {
        auto kidchains = kid->chains();
        for (size_t i = 0; i < kidchains.size(); ++i)
            kidchains[i].push_back(this);
        chains.insert(chains.end(), kidchains.begin(), kidchains.end());
    }
    return chains;
}

double PosetNode::x_avg() const {
    /* the average of the x-coordinates of the dots in the Coxeter diagram
     * which are included in the subgraph. For use positioning this node
     * in the Hasse diagram. */
    double sum = 0.0;
    auto vits = boost::vertices(cg);
    for (auto vit = vits.first; vit != vits.second; ++vit)
        sum += cg[*vit].x_coord;
    return sum/num_vertices(cg);
}

std::array<double,6> PosetNode::x_tuple() const {
    return {x_avg(),
        std::accumulate(parents.begin(), parents.end(), 0.0,
                [](double sum, const PosetNode* p){ return sum + p->x_avg(); })/parents.size(),
        std::accumulate(children.begin(), children.end(), 0.0,
                [](double sum, const PosetNode* p){ return sum + p->x_avg(); })/children.size(),
        (*std::min_element(parents.begin(), parents.end(),
                [](const PosetNode* a, const PosetNode* b){
                    return a->x_avg() < b->x_avg();
                }))->x_avg(),
        (*std::min_element(children.begin(), children.end(),
                [](const PosetNode* a, const PosetNode* b){
                    return a->x_avg() < b->x_avg();
                }))->x_avg(),
        static_cast<double>(cg[0].x_coord)}; //should be the least x_coord
}

/******************
 * FaceOrbitPoset *
 ******************/

FaceOrbitPoset::FaceOrbitPoset(const CoxeterGraph& cg) :
  nodes{num_vertices(cg) + 1},
  head{&inserter(nodes.back(), std::move(bitset{num_vertices(cg)}.set()), cg)} {
    genchildren();
}

void FaceOrbitPoset::genchildren() {
    for (int r = nodes.size() - 1; r > 0; --r) {
        for (auto& pn : nodes[r]) {
            // Try dropping each vertex in turn, and check if there
            // is a ringed node in every connected component.
            auto overt = pn.bs.find_first();
            auto vits = boost::vertices(pn.cg);
            for (auto v = vits.first; v != vits.second; ++v) {
                CoxeterGraph kid { pn.cg };
                boost::clear_vertex(*v, kid); //remove all edges to v
                boost::remove_vertex(*v, kid);
                if (allringed(kid)) {
                    const PosetNode& kidnode = inserter(nodes[r-1],
                             std::move(bitset{pn.bs}.reset(overt)), // clear bit overt
                                                              kid);
                    // if the bitset is already present, just add this parent
                    // to the existing node.
                    kidnode.parents.push_back(&pn);
                    pn.children.push_back(&kidnode);
                }
                overt = pn.bs.find_next(overt);
            }
        }
    }
}

void FaceOrbitPoset::to_tikz(TeXout& tex) const {
    const double width = proprange(get(&VertexProps::x_coord, head->cg));
    const double height = proprange(get(&VertexProps::y_coord, head->cg));
    // separation between the nodes:
    const double sep = width < 2.0 ? 1.0 : 1.5;
    const double yscale = max(height + 0.5, std::log2(max(2.0, width)));

    for (int y = nodes.size() - 1; y >= 0; --y) {
        const int num = nodes[y].size();
        vector<const PosetNode*> nds(num);
        std::transform(nodes[y].begin(), nodes[y].end(), nds.begin(),
                       [](const PosetNode& p){ return &p; });
        std::sort(nds.begin(), nds.end(), [](const PosetNode* a, const PosetNode* b)
                                            { return a->x_tuple() < b->x_tuple(); });
        for (size_t i = 0; i < nds.size(); ++i) {
            const double xpos = (width + sep)*(i - (num - 1)/2.0);
            tex << "\\node[draw] (n" << nds[i]->bs
                << ") at (" << xpos
                << ", " << y*yscale << ") {\n"
                << env_wrap{"tikzpicture"} << nds[i]->cg
                << "};\n";
            for (auto p : nds[i]->parents) {
                tex << "\\draw (n" << p->bs << ") -- (n" << nds[i]->bs << ");\n";
            }
        }
    }
}

OrbitGraph makeOrbit(const FaceOrbitPoset& hasse) {
    auto flagorbs = hasse.head->chains();
    OrbitGraph og {flagorbs.size()};
    // for each pair of chains in flagorbs that differ in exactly
    // the i-th entry, add an edge labeled i
    for (size_t i = 0; i < flagorbs.size(); ++i) {
        for (size_t j = i+1; j < flagorbs.size(); ++j) {
            auto difrank = diffinone(flagorbs[i], flagorbs[j]);
            if (difrank)
                boost::add_edge(i, j, {*difrank}, og);
        }
    }
    return og;
}

TeXout& operator<<(TeXout& tex, const OrbitGraph& og) {
    static bool inited {false};

    if (!inited) {
        tex.usepackage("tikz");
        tex.usetikzlibrary("graphs");
        tex.usetikzlibrary("graphdrawing");
        tex.usetikzlibrary("quotes");
        tex.addtopreamble("\\usegdlibrary{force}\n");
        tex.addtopreamble("\\tikzset{\n"
            "  graphs/edges={inner sep=1pt},\n"
            "  graphs/nodes={fill,circle,inner sep=1.6pt}\n"
            "}\n");
        inited = true;
    }

    auto edgits = boost::edges(og);
    if (edgits.first == edgits.second)
        return tex;
    tex << "\\graph[spring electrical layout,horizontal= 0 to 1] {\n";
    for (auto eit = edgits.first; eit != edgits.second; ++eit) {
        tex << boost::source(*eit, og) << "/ --[\"" << og[*eit].rank
            << "\"] " << boost::target(*eit, og) << "/;\n";
    }
    return tex << "};\n";
}

