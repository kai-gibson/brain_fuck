#include <iostream>
#include "tape.h"

Tape::Tape() {
    fwd.resize(size);
    bwd.resize(size);
}

void Tape::next(uint16_t amt) {
    head+=amt;
    if (head >= size) {
    size += 256; 
    fwd.resize(size); 
    bwd.resize(size); 
    }
}

void Tape::prev(uint16_t amt) {
    head-=amt;
    if(abs(head) >= size) {
        size += 256; 
        bwd.resize(size) ;
        fwd.resize(size) ;
    }
}

void Tape::inc(uint16_t amt) { this->val()+=amt; }
void Tape::dec(uint16_t amt) { this->val()-=amt; }

uint8_t& Tape::val() {
    return (head < 0 ? bwd[abs(head)] : fwd[head]);
}

void Tape::print() {
    std::cout << char(this->val());
}

void Tape::input() {
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
