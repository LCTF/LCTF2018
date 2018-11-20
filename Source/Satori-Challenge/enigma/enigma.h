#ifndef _ENIGMA
#define _ENIGMA

#include <vector>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include "signal.h"


class link_error: public std::logic_error{
public:
    explicit link_error(const std::string &what_arg):
        logic_error(what_arg) { }
    explicit link_error(const char *what_arg):
        logic_error(what_arg) { }
};

class Rotor{
public:
    Rotor(const std::vector<char> &keys, const std::unordered_map<int, int> &left);

    // signal & slot
    signal(upgrade);
    void turn();

    char key();
    bool set_rotor(char key);
    bool set_rotor(int count);
    // send index from left to right
    int link_data_left(int index);
    // send index from right to left
    int link_data_right(int index);

private:
    int turn_max;
    int turn_count;
    std::vector<char> index_key;
    std::unordered_map<int, int> left;
    std::unordered_map<int, int> right;
};

class Keyboard{
public:
    Keyboard(const std::vector<char> &keys);

    // signal
    signal(upgrade);

    int input(char key);
    char output(int index);

private:
    std::vector<char> index_key;
};

class Reflector{
public:
    Reflector(const std::unordered_map<int, int> &m);

    int link_data(int index);

private:
    std::unordered_map<int, int> maps;
};

class Enigma{
public:
    Enigma(const Keyboard &k, const std::vector<Rotor> &vr, const Reflector &r);

    void reset();
    std::string get_keys();
    bool set_keys(const std::string &key);

    std::string write(const std::string &s);

private:
    Keyboard keyboard;
    Reflector reflector;
    std::vector<Rotor> rotors;
};


#endif /* _ENIGMA */

