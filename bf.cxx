#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdint.h>
#include "tape.h"

struct Brace_Pair {
    uint32_t first;
    uint32_t last;
    bool nested;
};

std::vector<Brace_Pair> find_matching_braces(std::vector<uint8_t> v) {
    uint32_t depth = 0;
    uint32_t current_loop = 0;
    std::vector<Brace_Pair> pairs;

    for (uint32_t i=0; i< v.size(); i++) {
        switch (v[i]) {
            case '[': {
                pairs.push_back(
                    {i, 0, (depth > 0 ?true : false)}
                );
                if (depth == 0) { 
                    std::cout << "current_loop: " << pairs.size() << std::endl;
                    current_loop = pairs.size()-1; 
                }
                depth++; 
                break;
            }
            case ']': {
                if (depth > 1) {   
                    depth--;
                    std::cout << "depth>1, size: " << pairs.size()-1 << std::endl;
                    pairs[current_loop + depth].last = i;
                } 
                else if (depth == 1) {
                    depth--;
                    pairs[current_loop].last = i;
                } else {
                    pairs[current_loop].last = i;
                }

                break;
            default:
                break;
            }
        }
    }
    if (depth != 0) {
        throw std::runtime_error("Error, braces mismatched");
    }

    return pairs;
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

    // Get pairs of braces
    std::vector<Brace_Pair> pairs = find_matching_braces(cmd_list);

    for (uint32_t i = 0; i < pairs.size(); i++) {
        std::cout << "first: " << pairs[i].first
                  << " last: " << pairs[i].last 
                  << " nested: " << pairs[i].nested << std::endl;
    }

    Tape t;
    uint32_t loop_count = 0;
    bool loop_count_set = false;

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
                //std::cout << t.val() << "\n";
                t.print();
                break;
            case ',':
                t.input();
                break;
            case '[' : {
                if (!loop_count_set) {
                    loop_count_set = true;
                } else {
                    while (pairs[loop_count].first < i) {
                        loop_count++;
                    }
                }

                if (t.val() == 0) {
                    i = pairs[loop_count].last;
                    if (pairs[loop_count].nested) {
                        loop_count--;
                    } else {
                        loop_count++;
                    }
                }
                break;
            }
            case ']' : {
                if (t.val() != 0) {
                    i = pairs[loop_count].first;
                } else {
                    if (pairs[loop_count].nested) loop_count++;
                }
                break;
            }
            default:
                break;
        }
    }

    return 0;
}
