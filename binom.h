#ifndef NAM_BINOM_H
#define NAM_BINOM_H

#define BINOM_CHECK
/* If defined, binom(n,k) throws an std::overflow_error exception
 * when the result will not fit in 64 bits.
 * Otherwise, it just silently gives the wrong answer. */

#include <cstdint> // uint64_t
#include <vector>
#include <iosfwd>
#include <string>

/* Binomial coefficient, n choose k.
 * This produces the correct answer whenever it fits in 64 bits
 * (it does not have the internal overflow problems of a slightly more naive
 * implementation.) */
uint64_t binom(int n, int k);

/* Evaluate the polynomial in the "combinations basis":
 *  c_0 + c_1 (n C 1) + c_2 (n C 2) + ...
 *  where c_i is coef[i]. If offset is nonzero, each term is
 *  c_i (n C offset+i) instead.
 *  Currently, n must be non-negative, or the answer is 0. */
long binpoly(const std::vector<int>& coef, int n, int offset = 0);

/* Finds a binomial-bases polynomial generating the sequence,
 * if it exists and sufficiently many terms are given.
 * If no solution can be found, returns an empty vector. */
std::vector<int> seqsolver(std::vector<int> v, int start = 0);

/********* TeX stuff *********
 * Helpers to output a vector<int> as a polynomial
 * in the combinations basis, using TeXout */
struct binpolyTeX {
    const std::vector<int>& v;
    std::string varname;
    binpolyTeX(const std::vector<int>& vv, std::string vn = "x") :
        v(vv), varname(std::move(vn)) {}
};

class TeXout; //forward declaration: see TeXout.h

/* Use this like
 *  tex << binpolyTeX(myvector, 'n') << "blah";
 * The document should be in math mode! */
TeXout& operator<<(TeXout& tex, binpolyTeX bp);

std::ostream& operator<<(std::ostream& os, binpolyTeX bp);

#endif // NAM_BINOM_H
