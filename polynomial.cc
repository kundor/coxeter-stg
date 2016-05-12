#include "polynomial.h"
#include "TeXout.h"
//#include <ostream>
#include <boost/format.hpp>

using std::vector;

Polynomial binpolytopoly(vector<int> bp, Polynomial x) {
    if (x.size() == 0) {
        x.data().push_back(0);
        x.data().push_back(1);
    }
    if (bp.empty())
        return {};
    Polynomial top{1};
    int div{1};
    Polynomial result(bp[0]);
    for (int i = 1; i < bp.size(); ++i) {
        top *= x;
        x -= 1;
        div *= i;
        result += Rational(bp[i],div)*top;
    }
    return result;
}

/* Write the binomial-basis polynomial in bp to the Stream os.
 * bp.v must be nonempty. */
template <typename Stream>
static Stream& bpout(Stream& os, binpolyTeX bp, boost::format fmt) {
    if (bp.v.size() == 1)
        return os << bp.v[0];
    int i = bp.v.size() - 1;
    if (bp.v[i] == -1)
        os << '-';
    else if (bp.v[i] != 1)
        os << bp.v[i];
    os << (fmt % bp.varname % i).str();
    for (--i; i > 0; --i) {
        if (bp.v[i] > 1)
            os << " + " << bp.v[i] << (fmt % bp.varname % i).str();
        else if (bp.v[i] == 1)
            os << " + " << (fmt % bp.varname % i).str();
        else if (bp.v[i] == -1)
            os << " - " << (fmt % bp.varname % i).str();
        else if (bp.v[i] < -1)
            os << " - " << -bp.v[i] << (fmt % bp.varname % i).str();
    }
    if (bp.v[0] > 0)
        os << " + " << bp.v[0];
    else if (bp.v[0] < 0)
        os << " - " << -bp.v[0];
    return os;
}

TeXout& operator<<(TeXout& tex, binpolyTeX bp) {
    if (bp.v.empty())
        return tex;
    tex.usepackage("amsmath");
    return bpout(tex, bp, boost::format("\binom{%1%}{%2%}"));
}

std::ostream& operator<<(std::ostream& os, binpolyTeX bp) {
    if (bp.v.empty())
        return os;
    return bpout(os, bp, boost::format("(%1% C %2%)"));
}


