#ifndef NAM_BINOM_H
#define NAM_BINOM_H

#define BINOM_CHECK
// if defined, binom(n,k) throws an std::overflow_error exception
// when the result will not fit in 64 bits.

#include <cstdint> // uint64_t
#include <vector>

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
long long binpoly(std::vector<int> coef, int n, int offset=0);
 

#endif // NAM_BINOM_H
