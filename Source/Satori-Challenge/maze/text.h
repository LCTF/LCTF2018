#ifndef _TEXT
#define _TEXT

#include <iostream>

class Text{
public:
    Text();

    void input();
    void empty();
    void right();
    void wrong();
    std::string &output();

private:
    std::string _input;
    std::string _empty;
    std::string _right;
    std::string _wrong;
    std::string _output;
};

Text::Text():
    _input("Input your Flag:"),
    _empty("Error: empty input"),
    _right("OK, This is Flag"),
    _wrong("You have failed."),
    _output("IQURUEURYEU#WRTYIPUYRTI!WTYTE!WOR%Y$W#RPUEYQQ^EE")
{

}

void Text::input(){
    std::cout << _input << std::endl;
}

void Text::empty(){
    std::cout << _empty << std::endl;
}

void Text::right(){
    std::cout << _right << std::endl;
}

void Text::wrong(){
    std::cout << _wrong << std::endl;
}

std::string &Text::output(){
    return _output;
}


// default text instance
Text message_text;

#endif /* _TEXT */

