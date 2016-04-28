#ifndef NAM_BINOM_H
#define NAM_BINOM_H

#define BINOM_CHECK
// if defined, binom(n,k) throws an std::overflow_error exception
// when the result will not fit in 64 bits.

#include <cstdint> // uint64_t

/* Binomial coefficient, n choose k.
 * This produces the correct answer whenever it fits in 64 bits
 * (it does not have the internal overflow problems of a slightly more naive
 * implementation.) */
uint64_t binom(int n, int k);
 

#endif // NAM_BINOM_H
