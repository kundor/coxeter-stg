CCFLAGS= -std=gnu++14 -Wall -Wextra
OFLAGS= -march=native -O2 -flto
DFLAGS= -ggdb
CCFLAGS += $(DFLAGS)

ifeq ($(shell uname), Darwin)
LDFLAGS= -Wno-maybe-uninitialized -L/opt/local/lib
# at the link stage with -flto, g++ gives lots of spurious maybe-unitialized
# warnings from boost/graph/detail/adjacency_list.hpp
endif

truncations: truncations.o poset.o coxeter.o TeXout.o
	$(CXX) $(CCFLAGS) $^ $(LDFLAGS) -lboost_program_options-mt -o $@

truncations.o: ../truncations.cc ../poset.h ../coxeter.h ../TeXout.h
	$(CXX) $(CCFLAGS) -c $< 

countonly: countonly.o poset.o coxeter.o 
	$(CXX) $(CCFLAGS) $^ $(LDFLAGS) -o $@
# TeXout is not used here. On Mac OS X, the -dead_strip option
# culls references to it. On other platforms, something similar should
# be done, or else you have to link the unused TeXout.o

countonly.o: ../countonly.cc ../poset.h ../coxeter.h
	$(CXX) $(CCFLAGS) -c $<

poset.o: ../poset.cc ../poset.h ../coxeter.h ../optional.h ../TeXout.h
	$(CXX) $(CCFLAGS) -c $<

coxeter.o: ../coxeter.cc ../coxeter.h ../TeXout.h
	$(CXX) $(CCFLAGS) -c $<

TeXout.o: ../TeXout.cc ../TeXout.h
	$(CXX) $(CCFLAGS) -c $<
