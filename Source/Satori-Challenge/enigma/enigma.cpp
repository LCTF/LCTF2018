#include <iostream>
#include "enigma.h"


using std::string;
using std::vector;
using std::unordered_map;

/* ---------- Rotor ---------- */

Rotor::Rotor(const vector<char> &keys, const unordered_map<int, int> &maps):
    turn_count(0), index_key(keys), left(maps)
{
    turn_max = keys.size();

    for(auto &p : maps){
        right.emplace(p.second, p.first);
    }
}

void Rotor::turn(){
    turn_count += 1;
    
    if(turn_count >= turn_max){
        turn_count = 0;
        upgrade();
    }
}

char Rotor::key(){
    return index_key[turn_count];
}

bool Rotor::set_rotor(char key){
    for(int i = 0; i < index_key.size(); ++i){
        if(index_key[i] == key){
            turn_count = i;
            return true;
        }
    }

    return false;
}

bool Rotor::set_rotor(int index){
    if(index >= turn_max || index < 0){
        return false;
    }else{
        turn_count = index;
        return true;
    }
}

int Rotor::link_data_left(int index){
    if(index >= turn_max || index < 0){
        throw link_error("Rotor::link_data_left");
    }else{
        index = (index + turn_count) % turn_max;
    }

    try{
        return left.at(index);
    }catch(const std::out_of_range &oor){
        throw link_error("Rotor::link_data_left");
    }
}

int Rotor::link_data_right(int index){
    if(index >= turn_max || index < 0){
        throw link_error("Rotor::link_data_right");
    }else{
        index = (index + turn_count) % turn_max;
    }

    try{
        return right.at(index);
    }catch(const std::out_of_range &oor){
        throw link_error("Rotor::link_data_right");
    }
}

/* ---------- Keyboard ---------- */

Keyboard::Keyboard(const vector<char> &keys):
    index_key(keys)
{

}

int Keyboard::input(char key){
    for(int i = 0; i < index_key.size(); ++i){
        if(index_key[i] == key){
            return i;
        }
    }

    throw link_error("Keyboard::input");
}

char Keyboard::output(int index){
    if(index < 0 || index >= index_key.size()){
        throw link_error("Keyboard::output");
    }

    return index_key[index];
}

/* ---------- Reflector ---------- */

Reflector::Reflector(const unordered_map<int, int> &m):
    maps(m)
{

}

int Reflector::link_data(int index){
    try{
        return maps.at(index);
    }catch(const std::out_of_range &oor){
        throw link_error("Reflector::link_data");
    }
}

/* ---------- Enigma ---------- */

Enigma::Enigma(const Keyboard &k, const vector<Rotor> &vr, const Reflector &r):
    keyboard(k), rotors(vr), reflector(r)
{
    if(!rotors.empty()){
        //connect(keyboard, upgrade, rotors[0], turn);
        link_ss(keyboard.upgrade, rotors[0], &Rotor::turn);
        for(auto i = rotors.begin() + 1; i != rotors.end(); ++i){
            //connect_p(i - 1, upgrade, i, turn);
            link_ss((i-1)->upgrade, *i, &Rotor::turn);
        }
    }
}

void Enigma::reset(){
    for(auto &r : rotors){
        r.set_rotor(0);
    }
}

bool Enigma::set_keys(const string &key){
    if(key.size() != rotors.size()){
        return false;
    }

    // if set failed, reset rotors to ZERO
    for(int i = 0; i < rotors.size(); ++i){
        if(!rotors[i].set_rotor(key[i])){
            reset();
            return false;
        }
    }

    return true;
}

string Enigma::get_keys(){
    string keys;

    for(auto &r : rotors){
        keys += r.key();
    }

    return keys;
}

string Enigma::write(const string &s){
    int index;
    string result;

    for(char c : s){
        index = keyboard.input(c);
        for(auto &r : rotors){
            index = r.link_data_left(index);
        }
        index = reflector.link_data(index);
        for(auto &r : rotors){
            index = r.link_data_right(index);
        }
        result += keyboard.output(index);
        keyboard.upgrade();
    }

    return result;
}


