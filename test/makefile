CCFLAGS= -std=gnu++14 -Wall -Wextra -O2 -march=native

binomtest: binomtest.cc ../binom.cc
	$(CXX) $(CCFLAGS) -L/opt/local/lib -lgmp $< -o $@

perf-link: perf-link.cc ../binom.h binom.o
	$(CXX) $(CCFLAGS) $< binom.o -o $@

binpolytest: binpolytest.cc ../binom.h binom.o
	$(CXX) $(CCFLAGS) $< binom.o -o $@

binom.o: ../binom.cc ../binom.h
	$(CXX) $(CCFLAGS) -c $<

.cc:
	$(CXX) $(CCFLAGS) $< -o $@
