#ifndef SRC_H_
#define SRC_H_
#include <string.h>
typedef struct node* Node;
struct node{
    int x;
    int num;
    struct node * lchild;
    struct node * rchild;
};
Node create();
Node build(Node tree,int x);
void first(Node tree);
void middle(Node tree);
void last(Node tree);
void encrpyt1(char* flag);
void DES_encrypt(char* flag);
void matrix();
void matcheck();
void flag_num_check();
void decryptsmc();
void reversebit();
void check_reversebit();
void flag_num_check2();
void encrpyt2();
#endif
