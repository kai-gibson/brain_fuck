#include <stdint.h>
#include <memory>
#include <string>

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
    Tape();
    ~Tape();
    void next();
    void prev();
    void input();
    void print();
    void inc();
    void dec();
    uint8_t val();
};
