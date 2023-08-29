#include <iostream>
#include <stdint.h>
#include <new>
#include <cstring>

class Mem {
protected:
    uint8_t* mem;
    uint16_t pos;

    uint16_t m_size;

public:
    Mem(uint16_t size) {
        m_size = size;

        mem = (uint8_t*)malloc(size);
        std::memset(mem, 0, size);

        pos = (m_size/2)-1;

    }

    ~Mem() { free(mem); }

    void inc() { 
        if (mem[pos] != 0xFF) {
            mem[pos] += 1; 
        }
    }

    void dec() { 
        if (mem[pos] != 0) {
            mem[pos] -= 1; 
        }
    }

    // Need to add realloc if bounds are reached
    void next() { 
        if (pos <= m_size) {
            pos++; 
        }
    }

    void prev() { 
        if (pos != 0 && pos) {
            pos--; 
        }
    }

    uint16_t size() { return m_size; }
    uint16_t val() { return mem[pos]; }
};

int main() {
    Mem block(16);
    
    block.inc();
    block.inc();
    block.inc();

    std::cout << block.val() << std::endl;

    block.next();
    std::cout << block.val() << std::endl;
}
