#ifndef _MAZE
#define _MAZE

#include <string>
#include <array>

typedef unsigned char uchar;


class Map{
public:
    Map();
    Map(const Map &ins);
    Map &operator=(const Map &ins);

    Map(const char *list, int size);
    Map(std::string::iterator b, int size);

    virtual ~Map();

    std::string get();
    friend void addMap(Map &m, Map &d);
    friend void xorMap(Map &m, Map &d);

    virtual void map(uchar vaule) = 0;

protected:
    int count;
    uchar *data;
};

void addMap(Map &m, Map &d);
void xorMap(Map &m, Map &d);


class MPlus: public Map{
public:
    using Map::Map;

    // special copy constructor
    MPlus(const MPlus &ins);

    void map(uchar vaule) override;
};


class MXor: public Map{
public:
    using Map::Map;

    // special copy constructor
    MXor(const MXor &ins);

    void map(uchar vaule) override;
};


#endif /* _MAZE */

