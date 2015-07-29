CXX = g++
CXXFLAGS = -std=c++11 

ROOT_LIB := `root-config --libs --glibs`
ROOT_FLAGS := `root-config --cflags --ldflags`

DEPS = interface/CfgManager.h interface/WFClass.h interface/TekFilesManager.h 
DEPS_OBJ = lib/CfgManager.o lib/WFClass.o lib/TekFilesManager.o

lib/%.o: src/%.cc $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(ROOT_LIB) $(ROOT_FLAGS)

all: bin/dumper

bin/dumper: test/dumper.cpp $(DEPS_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(ROOT_LIB) $(ROOT_FLAGS)

clean:
	rm -f tmp/*
	rm -f lib/*.o
	rm -f bin/*
