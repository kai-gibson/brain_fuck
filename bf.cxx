#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "tape.h"

class Interpreter {
private:
    Tape t;
public:
    Interpreter(std::string filename) {
        //std::ifstream fs(filename);
        std::shared_ptr<std::ifstream> fs = std::make_shared<std::ifstream>(std::ifstream(filename));
        eval(fs);
    }

    template<typename T>
    void eval(std::shared_ptr<T>& i_stream) {
        static_assert(std::is_base_of_v<std::istream, T>, 
                      "arg must be istream");

        char ch;
        while (*i_stream >> std::skipws >> ch) {
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
                    //std::stringstream cmd_list; 
                    auto cmd_list = std::make_shared<std::stringstream>(
                            std::stringstream()
                            );
                    *cmd_list << find_match(i_stream);
                    if (cmd_list->str() != "") {
                        while (t.val() != 0) {
                            eval(cmd_list);
                            cmd_list->clear();
                            cmd_list->seekg(0); // rewind
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
    std::string find_match(std::shared_ptr<T>& i_stream) {
        static_assert(std::is_base_of_v<std::istream, T>, 
                      "arg must be istream");  

        std::string str;

        char ch;
        uint32_t depth = 1;
        while (depth != 0) {
            if (!(*i_stream >> std::skipws >> ch) && depth != 0) {
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
