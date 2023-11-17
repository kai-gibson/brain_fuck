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
    void jump_to(int32_t new_head);
    void inc(uint16_t amt);
    void dec(uint16_t amt);
    void set(uint8_t new_val);
    uint8_t& val();
    void print();
    void input();
    uint8_t& operator[] (int32_t i);
};
