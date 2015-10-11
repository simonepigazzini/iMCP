CXX = g++
CXXFLAGS = -std=c++11 
INCLUDE = -I"./"

ROOT_LIB := `root-config --libs --glibs`
ROOT_FLAGS := `root-config --cflags --ldflags`

DEPS = interface/CfgManager.h interface/WFClass.h interface/TekFilesManager.h interface/RecoTree.h interface/WFTree.h interface/H4Tree.h interface/HodoUtils.h
DEPS_OBJ = lib/CfgManager.o lib/WFClass.o lib/TekFilesManager.o lib/HodoUtils.o

lib/%.o: src/%.cc $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(INCLUDE) $(ROOT_LIB) $(ROOT_FLAGS)

all: bin/dumperTEST

bin/dumperTEK: test/dumperTEK.cpp $(DEPS_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(INCLUDE) $(ROOT_LIB) $(ROOT_FLAGS)

bin/dumperDIGI: test/dumperDIGI.cpp $(DEPS_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(INCLUDE) $(ROOT_LIB) $(ROOT_FLAGS)

bin/dumperTEST: test/dumperTEST.cpp $(DEPS_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(INCLUDE) $(ROOT_LIB) $(ROOT_FLAGS)

clean:
	rm -f tmp/*
	rm -f lib/*.o
	rm -f bin/*
