#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "maze.h"
#include "text.h"

using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::static_pointer_cast;

typedef shared_ptr<Map> spMap;


void maze();
vector<spMap> encrypto(vector<spMap> &msg);

int main(int argc, char *argv[]){

    maze();

    return 0;
}

void maze(){
    string input;
    vector<spMap> message;

    message_text.input();
    std::cin >> input;

    // empty check
    if(input.empty()){
        message_text.empty();
    }

    // padding to 4
    int size = input.length();
    if(size % 4 != 0){
        input += string(4 - size % 4, '0');
    }

    for(auto p = input.begin(); p != input.end(); p = p + 4){
        message.push_back(make_shared<MPlus>(p, 4));
    }

    vector<spMap> result = encrypto(message);

    string output;
    for(auto &p : result){
        output += p->get();
    }

#ifdef DEBUG
    std::cout << output << std::endl;
    std::cout << "input len: " << input.length() << std::endl;
    std::cout << "output len: " << output.length() << std::endl;

#else
    if(output != message_text.output()){
        message_text.wrong();
    }else{
        message_text.right();
    }
#endif

}

vector<spMap> encrypto(vector<spMap> &msg){
    vector<spMap> output;
    // I use CBC Block-Encrypto, but string "PCBC" is well to confuse them >_<
    spMap iv = make_shared<MXor>("PCBC", 4);
    spMap key = make_shared<MXor>("LCTF", 4);

    spMap tmp_msg;
    spMap tmp_key;
    for(auto &p : msg){
        tmp_msg = make_shared<MPlus>(*static_pointer_cast<MPlus, Map>(p));
        tmp_key = make_shared<MXor>(*static_pointer_cast<MXor, Map>(key));

        iv->map('5');
        tmp_msg->map('1');
        tmp_key->map('4');

        xorMap(*tmp_msg, *iv);
        addMap(*tmp_key, *tmp_msg);
        output.push_back(tmp_key);

        iv = tmp_key;
    }

    return output;
}

