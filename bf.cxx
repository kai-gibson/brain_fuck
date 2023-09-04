#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdint.h>
//#include "tape.h"

struct Node {
    u_int8_t val = 0;
    Node* prev = nullptr;
    Node* next = nullptr;
};

class Tape {
private:
    Node* h;
    std::string stdin = "";
    uint32_t current_char = 0; 

public:
    Tape() {
        h = (Node*)malloc(sizeof(Node));
        h->val = 0;
        h->prev = nullptr;
        h->next = nullptr;
    }
    ~Tape() {
        while (h->next != nullptr) { // Cycle to last node
            this->next(); 
        }
        while (h->prev != nullptr) { // free tape elements
            Node* tmp = h;
            this->prev();
            free(tmp);
        }
        free(h);  // 
    }

    void next() {
        if (h->next == nullptr) { 
            Node* tmp_next;// = new Node();
            tmp_next = (Node*)malloc(sizeof(Node));
            tmp_next->val = 0;
            tmp_next->prev = h;
            tmp_next->next = nullptr;

            h->next = tmp_next;
            h = h->next;
        } else {
            h = h->next;
        }
    }

    void prev() {
        if (h->prev == nullptr) { 
            Node* tmp_prev; 
            tmp_prev = (Node*)malloc(sizeof(Node));
            tmp_prev = new Node();
            tmp_prev->val = 0;
            tmp_prev->prev = nullptr;
            tmp_prev->next = h;

            h->prev = tmp_prev;
            h = h->prev;
        } else {
            h = h->prev;
        }
    }

    void input() {
        if (stdin == "") {
            std::cin >> stdin; 
        }
        if (current_char < stdin.length()) {
            if (std::isdigit(stdin[current_char])) {
                h->val = int(stdin[current_char] - '0'); // is this unsafe?
            } else {
                h->val = stdin[current_char];
            }
            current_char++;
        } else {
            h->val = 0;
        }
    }

    void inc() {
        h->val = h->val + 1;
    }

    void dec() {
        h->val = h->val - 1;
    }
    void print() {
        std::cout << h->val;
    }

    uint8_t val() {
        return h->val;
    }
};

struct Brace_Pair {
    uint32_t first;
    uint32_t last;
    uint32_t depth;
};

std::vector<Brace_Pair> find_matching_braces(std::vector<uint8_t> v) {
    uint32_t depth = 0;
    uint32_t current_loop = 0;
    std::vector<Brace_Pair> brace_pairs;

    for (uint32_t i=0; i< v.size(); i++) {
        switch (v[i]) {
            case '[': {
                brace_pairs.push_back({i, 0, depth});
                if (depth == 0) { 
                    std::cout << "current_loop: " << brace_pairs.size() << std::endl;
                    current_loop = brace_pairs.size()-1; 
                }
                depth++; 
                break;
            }
            case ']': {
                if (depth > 1) {   
                    depth--;
                    std::cout << "depth>1, size: " << brace_pairs.size()-1 << std::endl;
                    brace_pairs[current_loop + depth].last = i;
                } 
                else if (depth == 1) {
                    depth--;
                    brace_pairs[current_loop].last = i;
                } else {
                    brace_pairs[current_loop].last = i;
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

    return brace_pairs;
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
    std::vector<Brace_Pair> brace_pairs = find_matching_braces(cmd_list);

    for (uint32_t i = 0; i < brace_pairs.size(); i++) {
        std::cout << "first: " << brace_pairs[i].first
                  << " last: " << brace_pairs[i].last 
                  << " depth: " << brace_pairs[i].depth << std::endl;
    }

    uint32_t loop_count = 0;
    uint32_t outer_loop = 0;
    bool outer_loop_set = false;
    bool loop_count_set = false;
    uint8_t val = 0;

    for (uint32_t i=0; i<cmd_list.size(); i++) {
        switch (cmd_list[i]) {
            case '[': {
                if (!loop_count_set) {
                    loop_count_set = true;
                } else {
                    loop_count++;
                }

                if (val == 0) {
                    // skip loop
                    i = brace_pairs[loop_count].last;
                    // skip nested loops
                    while (brace_pairs[loop_count].last < i) {
                        loop_count++;
                    }
                } 
                break;
            }
            case ']': {
                if (val != 0) {
                    // Go back to loop start
                    i = brace_pairs[loop_count].first;
                    //if (i != 0) i--;
                } else {
                    if (brace_pairs[loop_count].depth > 0) {
                        // go back to previous loop 
                        // and don't allow iteration
                        loop_count_set = false;
                        loop_count--;
                    } else {
                        // skip nested loops
                        while (i < brace_pairs[loop_count].last) {
                            loop_count++;
                        }
                    }
                }
                break;
            }

            case '-': {
                val--;
                std::cout << "val -" << std::endl;
                break;
            }
            case '+': {
                val++;
                std::cout << "val +" << std::endl;
                break;
            }
            default:
                std::cout << cmd_list[i] << std::endl;
                break;
        }
    }

    return 0;
}
