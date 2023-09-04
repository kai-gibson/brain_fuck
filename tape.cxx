#include <iostream>
#include "tape.h"

#define WEAK std::weak_ptr<Node>
#define STRONG std::shared_ptr<Node>

Tape::Tape() { 
        h = std::make_shared<Node>(
            Node { 
                0, 
                std::weak_ptr<Node>(),
                std::weak_ptr<Node>()
            }
        ); 
}

void Tape::next() {
    try {
        if (std::get<WEAK>(h->next).expired()) {
            std::shared_ptr<Node> tmp_next = std::make_shared<Node>(
                Node{
                    0, 
                    std::weak_ptr<Node>(h), 
                    std::weak_ptr<Node>()
                }
            );

            h->next.emplace<STRONG>(tmp_next);
            h = std::get<STRONG>(h->next);
        } else {
            h = std::get<WEAK>(h->next).lock();
        }
    } catch (std::bad_variant_access const& ex) {
        h = std::get<STRONG>(h->next);
    }
}

void Tape::prev() {
    try {
        if (std::get<WEAK>(h->prev).expired()) { 
            std::shared_ptr<Node> tmp_prev = std::make_shared<Node>(
                Node{
                    0,
                    std::weak_ptr<Node>(),
                    std::weak_ptr<Node>(h)
                }
            );

            h->prev.emplace<STRONG>(tmp_prev);
            h = std::get<STRONG>(h->prev);
        } else {
            h = std::get<WEAK>(h->prev).lock();
        }
    } catch (std::bad_variant_access const& ex) {
        h = std::get<STRONG>(h->prev);
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

void Tape::print() { std::cout << h->val; }
void Tape::inc() { h->val = h->val + 1; }
void Tape::dec() { h->val = h->val - 1; }
uint8_t Tape::val() { return h->val; }
