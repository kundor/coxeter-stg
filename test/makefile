CCFLAGS= -std=gnu++14 -Wall -Wextra -O2 -march=native

binomtest: binomtest.cc ../binom.cc
	$(CXX) $(CCFLAGS) -L/opt/local/lib -lgmp $< -o $@
