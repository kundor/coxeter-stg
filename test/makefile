CCFLAGS= -std=gnu++14 -Wall -Wextra

binomtest: binomtest.cc ../binom.cc
	$(CXX) $(CCFLAGS) -L/opt/local/lib -lgmp $< -o $@
