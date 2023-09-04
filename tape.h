#include <stdint.h>
#include <memory>
#include <string>
#include <variant>

template<typename T>
using ptr = std::variant<std::weak_ptr<T>, std::shared_ptr<T>>;

struct Node {
    uint8_t val = 0;
    ptr<Node> prev = std::weak_ptr<Node>();
    ptr<Node> next = std::weak_ptr<Node>();
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
