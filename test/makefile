CCFLAGS= -std=gnu++14 -Wall -Wextra -O2 -march=native

binomtest: binomtest.cc ../binom.cc
	$(CXX) $(CCFLAGS) -L/opt/local/lib -dead_strip -lgmp $< -o $@

perf-link: perf-link.cc ../binom.h binom.o
	$(CXX) $(CCFLAGS) $< binom.o -dead_strip -o $@

binpolytest: binpolytest.cc ../binom.h binom.o
	$(CXX) $(CCFLAGS) $< binom.o -dead_strip -o $@

seqsolvertest: seqsolvertest.cc ../binom.h binom.o
	$(CXX) $(CCFLAGS) $< binom.o -dead_strip -o $@

binom.o: ../binom.cc ../binom.h
	$(CXX) $(CCFLAGS) -c $<

.cc:
	$(CXX) $(CCFLAGS) $< -o $@
