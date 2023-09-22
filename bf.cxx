#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdint.h>
#include <memory.h>
#include <chrono>
//#include "tape.h"

// TODO move this to tape.h
class Tape {
private:
    std::vector<uint8_t> fwd;
    std::vector<uint8_t> bwd;
    int32_t head;
    ssize_t size = 512;
    std::string stdin = "";
    uint32_t current_char = 0; 
public:
    Tape() {
        fwd.resize(size);
        bwd.resize(size);
    }

    void next() {
        if (head++ >= size) {
            size += 256; 
            fwd.resize(size); 
            bwd.resize(size); 
        }
    }

    void prev() {
        if(abs(head--) >= size) {
            size += 256; 
            bwd.resize(size) ;
            fwd.resize(size) ;
        }
    }

    void jump_to(int32_t new_head) {
        if(abs(new_head) >= size) {
            size += 256; 
            bwd.resize(size) ;
            fwd.resize(size) ;
        }

        head = new_head;
    }

    void inc() { this->val()++; }
    void dec() { this->val()--; }
    void set(uint8_t new_val) { this->val() = new_val; }

    uint8_t& val() {
        return (head < 0 ? bwd[abs(head)] : fwd[head]);
    }

    void print() {
        std::cout << char(this->val());
    }

    void input() {
        if (stdin == "") {
            std::cin >> stdin; 
        }
        if (current_char < stdin.length()) {
            if (std::isdigit(stdin[current_char])) {
                this->val() = int(stdin[current_char] - '0'); // is this unsafe?
            } else {
                this->val() = stdin[current_char];
            }
            current_char++;
        } else {
            this->val() = 0;
        }
    }
    
    uint8_t& operator[] (int32_t i) {
        return (i < 0 ? bwd[abs(i)] : fwd[i] );
    }
};

enum Token_Id {
    INC,
    DEC,
    LEFT,
    RIGHT,
    OPEN,
    CLOSE,
    PRINT,
    IN
};

struct Token {
    Token_Id id;
    uint32_t val;
};

std::vector<Token> tokenise(std::vector<uint8_t> v) {
    std::vector<Token> token_list;

    uint8_t op_count = 0;
    std::vector<uint32_t> open_braces;

    for (uint32_t i=0; i<v.size(); i++) {
        switch(v[i]) {
            // TODO make this DRY
            // TODO fix segfault (probably in bracket logic)
            // TODO make stacking instructions check they're not at 
            // end of list
            case '+':
                while (v[i+op_count] == '+') op_count++;
                token_list.push_back({INC, op_count});
                i += op_count;
                op_count = 0;
                break;
            case '-':
                while (v[i+op_count] == '-') op_count++;
                token_list.push_back({DEC, op_count});
                i += op_count;
                op_count = 0;
                break;
            case '<':
                while (v[i+op_count] == '<') op_count++;
                token_list.push_back({LEFT, op_count});
                i += op_count;
                op_count = 0;
                break;
            case '>':
                while (v[i+op_count] == '>') op_count++;
                token_list.push_back({RIGHT, op_count});
                i += op_count;
                op_count = 0;
                break;
            case '.':
                token_list.push_back({PRINT});
                break;
            case ',':
                token_list.push_back({IN});
                break;
            case '[' : 
                token_list.push_back({OPEN});
                open_braces.push_back(i);
                break;
            case ']' :
                if (open_braces.size() == 0) {
                    throw std::runtime_error("mismatched braces");
                }

                token_list[open_braces[open_braces.size()-1]].val = i; // open brace idx = close brace idx
                token_list.push_back({CLOSE, open_braces[open_braces.size()-1]});

                open_braces.pop_back();
                break;
            default:
                break;
        }
    }

    return token_list;
}

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
    std::cout << std::fixed;
    auto begin1 = std::chrono::high_resolution_clock::now();

    std::string filename;

    if (argc <= 1) {
        std::cout << "please specify file" << std::endl;
        return -1;
    } else {
        filename = argv[1];
    }

    // load file into vector
    std::ifstream fs(filename);
    std::vector<uint8_t> cmd_list(std::istreambuf_iterator<char>(fs), {});

    auto end1 = std::chrono::high_resolution_clock::now();
    std::cout << "t1: " << std::chrono::duration<double>(end1-begin1).count() << std::endl;

    auto begin2 = std::chrono::high_resolution_clock::now();

    // tokenise
    auto token_list = tokenise(cmd_list);

    std::vector<uint32_t> brace_map = match_braces(cmd_list);

    auto end2 = std::chrono::high_resolution_clock::now();
    std::cout << "t2: " << std::chrono::duration<double>(end2-begin2).count() << std::endl;
    Tape t;

    auto begin3 = std::chrono::high_resolution_clock::now();

    uint8_t inc_count = 0;
    uint8_t dec_count = 0;

    for (uint32_t i=0; i<cmd_list.size(); i++) {
        switch (cmd_list[i]) {
            case '+':
                t.inc();
                //if (cmd_list[i+1] == '+') {
                //    inc_count++;
                //} else if (inc_count) {
                //    inc_count++;
                //    t.set(t.val() + inc_count);
                //    inc_count = 0;
                //} else {
                //    t.inc();
                //}
                break;
            case '-':
                //if (cmd_list[i+1] == '-') {
                //    dec_count++;
                //} else if (dec_count) {
                //    dec_count++;
                //    t.set(t.val() - dec_count);
                //    dec_count = 0;
                //} else {
                //    t.dec();
                //}
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

    auto end3 = std::chrono::high_resolution_clock::now();

    std::cout << "t3: " << std::chrono::duration<double>(end3-begin3).count() << std::endl;
    return 0;
}
