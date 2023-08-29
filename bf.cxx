#include <iostream>
#include <string>
#include <fstream>
#include "tape.h"

int main(int argc, char** argv) {
    std::string filename;

    if (argc < 1) {
        std::cout << "please specify file" << std::endl;
        return -1;
    } else {
        //std::cout << argv[1] << std::endl;
        filename = argv[1];
    }

    std::ifstream file(filename);
    char ch;
    while ( file >> std::skipws >> ch) {
        std::cout << "ch: " << ch << std::endl;
    }
    
    Tape tape;
    tape.inc();
    tape.inc();
    std::cout << unsigned(tape.val()) << std::endl;
    tape.next();
    std::cout << unsigned(tape.val()) << std::endl;
    tape.prev();
    tape.dec();
    tape.dec();
    tape.dec();
    tape.dec();
    std::cout << unsigned(tape.val()) << std::endl;
}

