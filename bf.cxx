#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdint.h>
#include <memory.h>
#include <chrono>
#include "tape.h"

//#define BLOCK_SIZE 1024

// dangerous pointer idea
/*
class Tape {
private:
    uint8_t* ptr; 
    int32_t head;
    std::string stdin = "";
    uint32_t current_char = 0; 
public:
    Tape() {
        ptr = (uint8_t*)malloc(BLOCK_SIZE);
        memset(ptr, 0, sizeof(*ptr));         // Initialize array to zero
        head = BLOCK_SIZE/2;
    }
    void next() {
        ptr++;
        head++;

        if ( head >= 1024) {
            throw std::runtime_error("error, reached right "
                                     "bounds of malloced block");
        } 
    }
    void prev() {
        ptr--;
        head--;

        if (head <= 0) {
            throw std::runtime_error("error, reached left "
                                     "bound of malloced block");
        } 
    }
    void inc() {
        *ptr+=1;
    }
    void dec() {
        *ptr-=1;
    }

    void set(uint8_t val) {
        *ptr=val;
    }

    uint8_t val() {
        return *ptr;
    }
    void print() {
        std::cout << char(*ptr);
    }

    void input() {
        if (stdin == "") {
            std::cin >> stdin; 
        }
        if (current_char < stdin.length()) {
            if (std::isdigit(stdin[current_char])) {
                *ptr = int(stdin[current_char] - '0'); // is this unsafe?
            } else {
                *ptr = stdin[current_char];
            }
            current_char++;
        } else {
            *ptr = 0;
        }
    }
};
*/
/*
class Tape {
private:
    std::vector<uint8_t> fwd;
    std::vector<uint8_t> bwd;
    int32_t head;
    ssize_t size = 256;
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
        }
    }
    void prev() {
        if(abs(head--) >= size) {
            size += 256; 
            bwd.resize(size) ;
        }
    }
    void inc() {
        this->val()++;
    }
    void dec() {
        this->val()--;
    }
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
*/

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
    std::vector<uint32_t> brace_map = match_braces(cmd_list);

    auto end2 = std::chrono::high_resolution_clock::now();
    std::cout << "t2: " << std::chrono::duration<double>(end2-begin2).count() << std::endl;
    Tape t;

    auto begin3 = std::chrono::high_resolution_clock::now();

    std::ofstream out_fs;
    out_fs.open("out.cxx");

    for (uint32_t i=0; i<cmd_list.size(); i++) {
        switch (cmd_list[i]) {
            case '+':
                out_fs << "t.inc();\n";
                //t.inc();
                break;
            case '-':
                out_fs << "t.dec();\n";
                //t.dec();
                break;
            case '<':
                out_fs << "t.prev();\n";
                //t.prev();
                break;
            case '>':
                //t.next();
                out_fs << "t.next();\n";
                break;
            case '.':
                //std::cout << "val: " << t.val()) << "\n";
                out_fs << "t.print();\n";
                //t.print();
                break;
            case ',':
                out_fs << "t.input();\n";
                //t.input();
                break;
            case '[' : 
                // Could this accidentally accesss unallocd mem?
                out_fs << "while (t.val() != 0) {\n";
                //if (t.val() == 0) i = brace_map[i];
                break;
            case ']' :
                out_fs << "}\n";
                //if (t.val() != 0) i = brace_map[i];
                break;
            default:
                break;
        }
    }

    out_fs.close();

    auto end3 = std::chrono::high_resolution_clock::now();

    std::cout << "t3: " << std::chrono::duration<double>(end3-begin3).count() << std::endl;
    return 0;
}
