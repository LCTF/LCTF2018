#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstring>
#include "enigma.h"


using std::cerr;
using std::endl;

using std::string;
using std::vector;
using std::unordered_map;


const string sample_char = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const string ciphertext = "DQYHTONIJLYNDLA";

Enigma init_config();


int main(int argc, char *argv[]){
    string input_line;
    string output_line;

    Enigma box = init_config();

    std::cin >> input_line;

    try{
        output_line = box.write(input_line);
    }catch(const link_error &le){
        cerr << "write line: " << input_line << " failed" << endl;
        return -1;
    }

#ifdef DEBUG
    std::cout << output_line << endl;

#else
    if(output_line == ciphertext){
        std::cout << "OK, This is Flag" << endl;
    }else{
        std::cout << "You have failed" << endl;
    }
#endif

    return 0;
}


Enigma init_config(){
    vector<char> keys(sample_char.begin(), sample_char.end());

    unordered_map<int, int> map;
    for(int i = 0; i < 26; ++i){
        map[i] = i;
    }

    // keyboard
    Keyboard keyboard(keys);
    // reflector
    Reflector reflector(map);
    // rotors
    vector<Rotor> rotors = {
        Rotor(keys, map),
        Rotor(keys, map),
        Rotor(keys, map)
    };

    return Enigma(keyboard, rotors, reflector);
}


