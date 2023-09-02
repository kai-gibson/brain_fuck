#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdint.h>
//#include "tape.h"

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
    Tape() {
        h = (Node*)malloc(sizeof(Node));
        h->val = 0;
        h->prev = nullptr;
        h->next = nullptr;
    }
    ~Tape() {
        while (h->next != nullptr) { // Cycle to last node
            this->next(); 
        }
        while (h->prev != nullptr) { // free tape elements
            Node* tmp = h;
            this->prev();
            free(tmp);
        }
        free(h);  // 
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

    void input() {
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

    void inc() {
        h->val = h->val + 1;
    }

    void dec() {
        h->val = h->val - 1;
    }
    void print() {
        std::cout << h->val;
    }

    uint8_t val() {
        return h->val;
    }
};

class Interpreter {
private:
    Tape t;
public:
    Interpreter(std::string filename) {
        std::ifstream fs(filename);
        eval(fs);
    }

    template<typename T>
    void eval(T& i_stream) {
        static_assert(std::is_base_of_v<std::istream, T>, 
                      "arg must be istream");

        char ch;
        while (i_stream >> std::skipws >> ch) {
            switch (ch) {
                case '+':
                    t.inc();
                    break;
                case '-':
                    t.dec();
                    break;
                case '<':
                    t.prev();
                    break;
                case '>':
                    t.next();
                    break;
                case '.':
                    //std::cout << t.val() << "\n";
                    t.print();
                    break;
                case ',':
                    t.input();
                    break;
                case '[': {
                    std::stringstream cmd_list; 
                    //auto cmd_list_ptr = std::make_shared<std::stringstream>(cmd_list);
                    cmd_list << find_match(i_stream);
                    if (cmd_list.str() != "") {
                        while (t.val() != 0) {
                            eval(cmd_list);
                            cmd_list.clear();
                            cmd_list.seekg(0); // rewind
                        }
                    }
                } break;

                case ']':
                    throw std::runtime_error("Error: ']' with no "
                                             " matching '['");
                    break;
                default:
                    break;
            }
        }
    }

    template<typename T>
    std::string find_match(T& i_stream) {
        static_assert(std::is_base_of_v<std::istream, T>, 
                      "arg must be istream");  

        std::string str;

        char ch;
        uint32_t depth = 1;
        while (depth != 0) {
            if (!(i_stream >> std::skipws >> ch) && depth != 0) {
                throw std::runtime_error("Error: couldn't find matching "
                                         "']' for '['"); 
            }
            switch (ch) {
                case ']':
                    if (depth != 1) str += ch;
                    depth -= 1;
                    break;
                case '[':
                    depth += 1;
                    [[fallthrough]];
                default:
                    str += ch;
                    break;
            }
        }
        return str;
    }
};

int main(int argc, char** argv) {
    std::string filename;

    if (argc <= 1) {
        std::cout << "please specify file" << std::endl;
        return -1;
    } else {
        filename = argv[1];
    }

    Interpreter interp(filename);
    //Tape tape;
    //std::ifstream file(filename);

    //char ch;
    return 0;
}
