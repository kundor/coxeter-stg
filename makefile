CCFLAGS= -std=gnu++14 -Wall -Wextra
OFLAGS= -march=native -O2 -flto
DFLAGS= -ggdb
CCFLAGS += $(OFLAGS)

ifeq ($(shell uname), Darwin)
LDFLAGS= -dead_strip -Wl,-S,-x
endif

countonly: countonly.o poset.o coxeter.o 
	$(CXX) $(CCFLAGS) $^ $(LDFLAGS) -o $@
# TeXout is not used here. On Mac OS X, the -dead_strip options
# culls references to it. On other platforms, something similar should
# be done, or else you have to link the unused TeXout.o

countonly.o: countonly.cc poset.h coxeter.h
	$(CXX) $(CCFLAGS) -c $<

onetruncation: onetruncation.o poset.o coxeter.o TeXout.o
	$(CXX) $(CCFLAGS) $^ $(LDFLAGS) -o $@

onetruncation.o: onetruncation.cc poset.h coxeter.h TeXout.h
	$(CXX) $(CCFLAGS) -c $< 

alltruncations: alltruncations.o poset.o coxeter.o TeXout.o
	$(CXX) $(CCFLAGS) $^ $(LDFLAGS) -o $@

alltruncations.o: alltruncations.cc poset.h coxeter.h TeXout.h
	$(CXX) $(CCFLAGS) -c $<

poset.o: poset.cc poset.h coxeter.h optional.h TeXout.h
	$(CXX) $(CCFLAGS) -c $<

coxeter.o: coxeter.cc coxeter.h TeXout.h
	$(CXX) $(CCFLAGS) -c $<

TeXout.o: TeXout.cc TeXout.h
	$(CXX) $(CCFLAGS) -c $<
