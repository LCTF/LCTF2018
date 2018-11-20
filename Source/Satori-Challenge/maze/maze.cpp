#include <string>
#include "maze.h"

#ifdef DEBUG
#include <iostream>
#endif

using std::string;

/* ---------- Map ---------- */

Map::Map():
    data(nullptr), count(0)
{

}

Map::Map(const Map &ins):
    data(new uchar[ins.count]), count(ins.count)
{
    for(int i = 0; i < count; ++i){
        data[i] = ins.data[i];
    }
}

Map &Map::operator=(const Map &ins){
    this->data = new uchar[ins.count];
    this->count = ins.count;

    for(int i = 0; i < count; ++i){
        data[i] = ins.data[i];
    }

    return *this;
}

Map::Map(const char *list, int size):
    data(new uchar[size]), count(size)
{
    for(int i = 0; i < size; ++i){
        data[i] = static_cast<uchar>(list[i]);
    }
}

Map::Map(string::iterator b, int size):
    data(new uchar[size]), count(size)
{
    for(int i = 0; i < size; ++i, ++b){
        data[i] = static_cast<uchar>(*b);
    }
}

Map::~Map(){
    delete[] data;
}


const char *base16_table = "QWERTYUIOP!@#$%^";

string Map::get(){
    uchar d;
    string output;

    // base16
    for(int i = 0; i < count; ++i){
        d = this->data[i];
        output += base16_table[(d >> 4) & 0x0F];
        output += base16_table[d & 0x0F];
    }

    return output;
}

/* ---------- MPlus ---------- */

void MPlus::map(uchar value){
#ifdef DEBUG
    std::cout << "MPlus map" << std::endl;
#endif

    for(int i = 0; i < count; ++i){
        data[i] = data[i] ^ value ^ 0x11;
    }
}

MPlus::MPlus(const MPlus &ins):
    Map(ins)
{
#ifdef DEBUG
    std::cout << "MPlus copy-constructor" << std::endl;
#endif

    // SPECIAL CONDUCT !!!
    this->map(1);
}


/* ---------- MXor ---------- */

void MXor::map(uchar value){
#ifdef DEBUG
    std::cout << "MXor map" << std::endl;
#endif

    for(int i = 0; i < count; ++i){
        data[i] = data[i] ^ value;
    }
}

MXor::MXor(const MXor &ins):
    Map(ins)
{
#ifdef DEBUG
    std::cout << "MXor copy-constructor" << std::endl;
#endif

    // SPECIAL CONDUCT !!!
    this->map(0x35);
}

/* ---------- Friend Function ---------- */


// m.data = add(m.data, d.data)
void addMap(Map &m, Map &d){
#ifdef DEBUG
    std::cout << "add Map" << std::endl;
#endif

    int min = m.count < d.count? m.count : d.count;

    for(int i = 0; i < min; ++i){
        m.data[i] = m.data[i] ^ d.data[i] ^ 0x11;
    }
}

// m.data = xor(m.data, d.data)
void xorMap(Map &m, Map &d){
#ifdef DEBUG
    std::cout << "xor Map" << std::endl;
#endif

    int min = m.count < d.count? m.count : d.count;

    for(int i = 0; i < min; ++i){
        m.data[i] = m.data[i] ^ d.data[i];
    }
}


