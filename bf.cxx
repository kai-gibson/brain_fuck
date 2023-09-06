#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdint.h>
#include "tape.h"

//std::unordered_map<uint32_t, uint32_t> 
std::vector<uint32_t>
match_braces(std::vector<uint8_t> v) {
    //std::unordered_map<uint32_t, uint32_t> map;
    std::vector<uint32_t> map;
    map.resize(v.size());
    //map = {};
    std::vector<uint32_t> open;

    for (uint32_t i=0; i< v.size(); i++) { // Get all brackets
        switch(v[i]) {
            case '[' :
                open.push_back(i);
                break;
            case ']' :
                if (open.size() == 0) {
                    throw std::runtime_error("mismatched braces");
                }
                map[open[open.size()-1]] = i;
                map[i] = open[open.size()-1];
                open.pop_back();
                break;
            default:
                break;
        }
    }

    if (open.size() != 0) {
        throw std::runtime_error("mismatched braces");
    } 
    return map;
}

int main(int argc, char** argv) {
    std::string filename;

    if (argc <= 1) {
        std::cout << "please specify file" << std::endl;
        return -1;
    } else {
        filename = argv[1];
    }

    // load file into vector
    std::ifstream fs(filename);
    std::vector<uint8_t> cmd_list(
            std::istreambuf_iterator<char>(fs), {});

    std::vector<uint32_t> brace_map = match_braces(cmd_list);

    Tape t;

    for (uint32_t i=0; i<cmd_list.size(); i++) {
        switch (cmd_list[i]) {
            case '+':
                t.inc();
                break;
            case '-':
                t.dec();
                break;
            case '<':
                t.prev();
                break;
            case '>':
                t.next();
                break;
            case '.':
                //std::cout << "val: " << t.val()) << "\n";
                t.print();
                break;
            case ',':
                t.input();
                break;
            case '[' : 
                // Could this accidentally accesss unallocd mem?
                if (t.val() == 0) i = brace_map[i];
                break;
            case ']' :
                if (t.val() != 0) i = brace_map[i];
                break;
            default:
                break;
        }
    }

    return 0;
}
