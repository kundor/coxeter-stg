#ifndef NAM_POLYNOMIAL_H
#define NAM_POLYNOMIAL_H
#include <boost/rational.hpp>
#include <boost/math/tools/polynomial.hpp>
#include <iosfwd>

typedef boost::rational<int> Rational;
typedef boost::math::tools::polynomial<Rational> Polynomial;

/* Might be nice to have implicit conversion from Rational to a degree-0 poly
 * (it's explicit, from arbitrary type?!, in boost::math)
    polynomial(Rational r) : coeff{r} {} */

Polynomial binpolytopoly(std::vector<int> bp, Polynomial x = {});
/* In Boost 1.61, we can have the default polynomial be "x" with
 * Polynomial x = {0, 1}
 * until then, we default to the zero polynomial, and check in the function
 * to replace it with {0,1}. */

/********* TeX stuff *********
 * Helpers to output a vector<int> as a polynomial
 * in the combinations basis, using TeXout */
struct binpolyTeX {
    const std::vector<int>& v;
    const char* varname;
    binpolyTeX(const std::vector<int>& vv, const char* vn = "x") :
        v(vv), varname(vn) {}
};

class TeXout; //forward declaration: see TeXout.h

/* Use this like
 *  tex << binpolyTeX(myvector, 'n') << "blah";
 * The document should be in math mode! */
TeXout& operator<<(TeXout& tex, binpolyTeX bp);

std::ostream& operator<<(std::ostream& os, binpolyTeX bp);

#endif //NAM_POLYNOMIAL_H
