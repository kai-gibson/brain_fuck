#include <stdint.h>
#include <memory>

struct Node {
    uint8_t val;
    std::shared_ptr<Node> prev;
    std::shared_ptr<Node> next;
};

class Tape {
private:
    std::shared_ptr<Node> h;
public:
    Tape();
    void next();
    void prev();
    void inc();
    void dec();
    uint8_t val();
};
