CXX ?= /usr/bin/g++
#CXXFLAGS = -fsanitize=address,undefined,leak -O0 -g3 -pthread -gdwarf-4 -DLinux -fPIC -Wno-deprecated -pipe -fno-elide-type -fdiagnostics-show-template-tree -Wall -Werror -Wextra -Wpedantic -Wvla -Wextra-semi -Wnull-dereference -Wswitch-enum -fvar-tracking-assignments -Wduplicated-cond -Wduplicated-branches -rdynamic -Wsuggest-override -MMD -MP
CXXFLAGS = -O3


brain_fuck: bf.cxx tape.cxx
	${CXX} $(CXXFLAGS) bf.cxx -o brain_fuck 

run: brain_fuck
	./brain_fuck test.bf
