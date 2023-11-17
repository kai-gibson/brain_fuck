#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>

class Tape {
private:
    std::vector<uint8_t> fwd;
    std::vector<uint8_t> bwd;
    int32_t head;
    ssize_t size = 512;
    std::string stdin = "";
    uint32_t current_char = 0; 
public:
    Tape();
    void next(uint16_t amt);
    void prev(uint16_t amt);
    void inc(uint16_t amt);
    void dec(uint16_t amt);
    uint8_t& val();
    void print();
    void input();
};
