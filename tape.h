#include <stdint.h>
#include <memory>
#include <string>

struct Node {
    uint8_t val = 0;
    std::shared_ptr<Node> prev = nullptr;
    std::shared_ptr<Node> next = nullptr;
};

class Tape {
private:
    std::shared_ptr<Node> h;
    std::string stdin = "";
    uint32_t current_char = 0; 
public:
    Tape();
    void next();
    void prev();
    void input();
    void print();
    void inc();
    void dec();
    uint8_t val();
};
