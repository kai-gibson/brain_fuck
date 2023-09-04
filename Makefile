CXX ?= g++
CXXFLAGS= -fsanitize=address,undefined,leak -O0 -g3 -pthread -gdwarf-4 -DLinux -fPIC -Wno-deprecated -pipe -fno-elide-type -fdiagnostics-show-template-tree -Wall -Werror -Wextra -Wpedantic -Wvla -Wextra-semi -Wnull-dereference -Wswitch-enum -fvar-tracking-assignments -Wduplicated-cond -Wduplicated-branches -rdynamic -Wsuggest-override -MMD -MP

brain_fuck: bf.cxx tape.cxx
	${CXX} $(CXXFLAGS) bf.cxx tape.cxx -o brain_fuck

run: brain_fuck
	./brain_fuck test.bf

clean:
	rm brain_fuck
