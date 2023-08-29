#include "tape.h"

Tape::Tape() { h = std::make_shared<Node>(Node()); }

void Tape::next() {
    if (h->next == nullptr) { 
        std::shared_ptr<Node> tmp_next = std::make_shared<Node>(
            Node{0, h, nullptr}
        );

        h->next = tmp_next;
        h = h->next;
    } else {
        h = h->next;
    }
}

void Tape::prev() {
    if (h->prev == nullptr) { 
        std::shared_ptr<Node> tmp_prev = std::make_shared<Node>(
            Node{0, nullptr, h}
        );

        h->prev = tmp_prev;
        h = h->prev;
    } else { 
        h = h->prev; 
    }
}

void Tape::inc() { h->val = h->val + 1; }
void Tape::dec() { h->val = h->val - 1; }
uint8_t Tape::val() { return h->val; }
