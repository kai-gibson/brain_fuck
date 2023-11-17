#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdint.h>
#include <memory.h>
#include <chrono>
#include "tape.h"

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
    int32_t ins_count = 0;
    std::vector<uint32_t> open_braces;

    for (uint32_t i=0; i<v.size(); i++) {
        switch(v.at(i)) {
            // TODO make this DRY
            // TODO fix segfault (probably in bracket logic)
            // TODO make stacking instructions check they're not at 
            // end of list
            case '+':
                while (v.at(i+op_count) == '+') op_count++;
                token_list.push_back({INC, op_count});
                i += op_count-1;
                op_count = 0;
                ins_count++; 
                break;
            case '-':
                while (v.at(i+op_count) == '-') op_count++;
                token_list.push_back({DEC, op_count});
                i += op_count-1;
                op_count = 0;
                ins_count++; 
                break;
            case '<':
                while (v.at(i+op_count) == '<') op_count++;
                token_list.push_back({LEFT, op_count});
                i += op_count-1;
                op_count = 0;
                ins_count++; 
                break;
            case '>':
                while (v.at(i+op_count) == '>') op_count++;
                token_list.push_back({RIGHT, op_count});
                i += op_count-1;
                op_count = 0;
                ins_count++; 
                break;
            case '.':
                token_list.push_back({PRINT, 0});
                ins_count++; 
                break;
            case ',':
                token_list.push_back({IN, 0});
                ins_count++; 
                break;
            case '[' : 
                token_list.push_back({OPEN, 0});
                open_braces.push_back(ins_count);
                ins_count++; 
                break;
            case ']' :
                if (open_braces.size() == 0) {
                    throw std::runtime_error("mismatched braces");
                }

                token_list.at(open_braces.back()).val = ins_count; // open brace idx = close brace idx
                token_list.push_back({CLOSE, open_braces.back()});
                open_braces.pop_back();
                ins_count++; // keep track of token list position
                break;
            default:
                break;
        }
    }

    return token_list;
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

    auto end2 = std::chrono::high_resolution_clock::now();
    std::cout << "t2: " << std::chrono::duration<double>(end2-begin2).count() << std::endl;
    Tape t;

    auto begin3 = std::chrono::high_resolution_clock::now();

    for (uint32_t i=0; i<token_list.size(); i++) {
        switch (token_list.at(i).id) {
            case INC:
                t.inc(token_list.at(i).val);
                break;
            case DEC:
                t.dec(token_list.at(i).val);
                break;
            case LEFT:
                t.prev(token_list.at(i).val);
                break;
            case RIGHT:
                t.next(token_list.at(i).val);
                break;
            case PRINT:
                t.print();
                break;
            case IN:
                t.input();
                break;
            case OPEN:
                if (t.val() == 0) i = token_list.at(i).val;
                break;
            case CLOSE:
                if (t.val() != 0) i = token_list.at(i).val;
                break;
            default:
                break;
        }
    }

    auto end3 = std::chrono::high_resolution_clock::now();

    std::cout << "t3: " << std::chrono::duration<double>(end3-begin3).count() << std::endl;
    return 0;
}
