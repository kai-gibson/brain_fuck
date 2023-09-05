#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdint.h>
#include <unordered_map>
#include "tape.h"

struct Brace_Pair {
    uint32_t first;
    uint32_t last;
    bool nested;
};

std::unordered_map<uint32_t, uint32_t> 
match_braces(std::vector<uint8_t> v) {
    std::unordered_map<uint32_t, uint32_t> map;
    std::vector<uint32_t> open;
    std::vector<uint32_t> close;

    for (uint32_t i=0; i< v.size(); i++) { // Get all brackets
        switch(v[i]) {
            case '[' :
                open.push_back(i);
                break;
            case ']' :
                close.push_back(i);
                break;
            default:
                break;
        }
    }

    /*
    std::cout << "open : ";
    for (uint32_t i=0; i < open.size(); i++) {
        std::cout << open[i] << ", "; 
    }
    std::cout << "\n";

    std::cout << "close : ";
    for (uint32_t i=0; i < close.size(); i++) {
        std::cout << close[i] << ", "; 
    }
    std::cout << "\n";
    */

    if (open.size() != close.size()) {
        throw std::runtime_error("mismatch of '[' and ']'");
    }

    for (uint32_t i=open.size(); i-- > 0;) {
        for (uint32_t v=0; v < close.size(); v++) {
            if (open[i] < close[v]) {
                //std::cout << "matched : " << open[i] << " = " 
                //          << close[v] << std::endl;
                map[open[i]] = close[v]; 
                map[close[v]] = open[i]; 
                close[v] = 0;
                break;
            }
        }
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


    //for (uint32_t i = 0; i < pairs.size(); i++) {
    //    std::cout << "first: " << pairs[i].first
    //              << " last: " << pairs[i].last 
    //              << " nested: " << pairs[i].nested << std::endl;
    //}

    // Get pairs of braces
    std::unordered_map<uint32_t, uint32_t> brace_map = match_braces(cmd_list);
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
