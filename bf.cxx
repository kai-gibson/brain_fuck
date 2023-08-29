#include <iostream>
#include <string>
#include <fstream>
#include "tape.h"

#define COUT(str) std::cout << str << std::endl

int main(int argc, char** argv) {
    std::string filename;

    if (argc < 1) {
        std::cout << "please specify file" << std::endl;
        return -1;
    } else {
        filename = argv[1];
    }

    Tape tape;

    std::ifstream file(filename);
    char ch;
    while ( file >> std::skipws >> ch) {
        //std::cout << "ch: " << ch << std::endl;

        switch (ch) {
            case '+':
                tape.inc();
                break;
            case '-':
                tape.dec();
                break;
            case '<':
                tape.prev();
                break;
            case '>':
                tape.next();
                break;
            case '.':
                COUT(unsigned(tape.val()));
                break;
            default:
                break;
        }
    }
}

