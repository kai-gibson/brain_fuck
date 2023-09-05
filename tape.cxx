#include <iostream>
#include "tape.h"

Tape::Tape() {
        h = (Node*)malloc(sizeof(Node));
        h->val = 0;
        h->prev = nullptr;
        h->next = nullptr;
}

Tape::~Tape() {
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

void Tape::next() {
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

void Tape::prev() {
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

void Tape::input() {
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

void Tape::inc() { h->val = h->val + 1; }
void Tape::dec() { h->val = h->val - 1;}
void Tape::print() { std::cout << char(h->val); }
uint8_t Tape::val() { return h->val; }
