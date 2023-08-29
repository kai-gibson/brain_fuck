#include <iostream>
#include <memory>

struct Node {
    u_int8_t val = 0;
    Node* prev = nullptr;
    Node* next = nullptr;
};

class Tape {
private:
    Node* h;

public:
    Tape() {
        h = (Node*)malloc(sizeof(Node));
        h->val = 0;
        h->prev = nullptr;
        h->next = nullptr;
    }
    ~Tape() {
        while (h->next != nullptr) {
            this->next(); 
        }
        while (h->prev != nullptr) {
            Node* tmp = h;
            this->prev();
            free(tmp);
        }
        free(h);
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

    void inc() {
        h->val = h->val + 1;
    }

    void dec() {
        h->val = h->val - 1;
    }

    uint8_t val() {
        uint8_t ret = h->val;
        return ret;
    }
};

int main() {
    Tape tape;
    tape.inc();
    tape.inc();
    std::cout << unsigned(tape.val()) << std::endl;
    tape.next();
    std::cout << unsigned(tape.val()) << std::endl;
    tape.prev();
    tape.dec();
    tape.dec();
    tape.dec();
    tape.dec();
    std::cout << unsigned(tape.val()) << std::endl;
}

