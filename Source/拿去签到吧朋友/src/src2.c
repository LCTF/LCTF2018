#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdbool.h>
#include <tlhelp32.h>
#include "src.h"
#include "des_ecb_enc.h"

extern struct node Tree[150];
extern int size;
extern int flag_num; 
extern char key[40];
extern int flag_num_merber[36];
extern int count2; 
extern unsigned char des[50];
extern int mat[50];
extern int mat_enc[36];
extern int mat_check[36];
extern char *reversebit_adr;
int flag_num_member_check[18] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};
int flag_num_member_check2[18] = {18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35};

unsigned char check2_data[36] = {124,129,97,153,103,155,20,234,104,135,16,236,22,249,7,242,15,243,3,244,51,207,39,198,38,195,61,208,44,210,35,222,40,209,1,230};
Node create()
{
    Tree[size].lchild=NULL;
    Tree[size].rchild=NULL;
    return &Tree[size++];
}

/*建立二叉排序树*/
Node build(Node tree,int x)
{
    if(tree==NULL){
        tree=create();
        tree->x=x;
        tree->num = flag_num;
        flag_num++;
        return tree;
    }
	else if(x>tree->x){
        tree->rchild=build(tree->rchild,x);
    }
    else if(x<tree->x){
        tree->lchild=build(tree->lchild,x);
    }
    return tree;
}
/*先序遍历并存储*/
void first(Node tree)
{
//    printf("%d ",tree->x);
    key[count2] = tree->x;
    flag_num_merber[count2] = tree->num;
//    printf("%d ",flag_num_merber[count2]);
    count2++;
    if(tree->lchild!=NULL)
        first(tree->lchild);
    if(tree->rchild!=NULL)
        first(tree->rchild);
 
}
/*中序遍历并存储*/
void middle(Node tree)
{
    if(tree->lchild!=NULL)
         middle(tree->lchild);
//    printf("%d ",tree->x);
    key[count2] = tree->x;
    count2++;
    if(tree->rchild!=NULL)
         middle(tree->rchild);
}
/*后序遍历并存储*/
void last(Node tree)
{
    if(tree->lchild!=NULL)
        last(tree->lchild);
    if(tree->rchild!=NULL)
    	last(tree->rchild);
//    printf("%d ",tree->x);
    key[count2] = tree->x;
    flag_num_merber[count2] = tree->num;
//    printf("%d ",flag_num_merber[count2]);
    count2++;
}

/*DES加密*/
void DES_encrypt(char* flag){
	int j = 0;
	char key_enc[8] = "fa1conn";
	char output[37] = {0};
	for(j = 0;j < 36;j += 8){ 
        SetKey(key_enc);
		Des_Run(&output[j], &flag[j]); 
	}
}

/*矩阵加密*/
void matrix()
{	
	memset(mat,0,sizeof(mat)/sizeof(int));
	for(int i=0;i<6;i++){		//列
		for(int j=0;j<6;j++){	//行
			for(int k=0;k<6;k++){
				mat[i+j*6] += des[j*6+k]*mat_enc[k*6+i];
			}
//			printf("%d.",mat[i+j*6]);
		}		
	}
}

/*DES验证*/
void matcheck()
{
	for(int i=0;i<36;i++){
		if(mat_check[i]!=mat[i]){ 
			printf("error\n"); 
			exit(0);
		}	
	}
	if(des[36]!=115||des[37]!=60||des[38]!=245||des[39]!=124){
		printf("error");
		exit(0);
	}
}


void flag_num_check()
{
    if(flag_num_merber[0] != flag_num_member_check[0]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[1] != flag_num_member_check[1]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[14] != flag_num_member_check[2]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[12] != flag_num_member_check[3]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[17] != flag_num_member_check[4]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[18] != flag_num_member_check[5]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[19] != flag_num_member_check[6]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[27] != flag_num_member_check[7]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[28] != flag_num_member_check[8]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[2] != flag_num_member_check[9]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[15] != flag_num_member_check[10]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[20] != flag_num_member_check[11]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[31] != flag_num_member_check[12]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[29] != flag_num_member_check[13]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[30] != flag_num_member_check[14]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[16] != flag_num_member_check[15]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[13] != flag_num_member_check[16]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[5] != flag_num_member_check[17]){
        printf("error\n");
        exit(0);
    }
}

void encrpyt1(char* flag){
    DES_encrypt(flag);
	matrix();
	matcheck();
    flag_num_check();
}

void decryptsmc(){
    int seed = 0;
    DWORD flOldProtect;
    for(int i = 0;i < 36;i++){
        seed += key[i];
    }
//    printf("%d",seed);
    srand(seed);    //3093
	VirtualProtect(reversebit,1, 0x40u, &flOldProtect);
    for(int i = 0;i < 119;i++){
        reversebit_adr[i] ^= (rand()%256);
    }
    if(reversebit_adr[0] != 0x55){
        printf("error.\n");
        exit(0);
    }
}

/*按位取反*/
void reversebit(){
    
    __asm__("mov eax,0x401E88\n");//花指令
    __asm__ ("jmp eax\n");

    for(int i = 0;i < 36;i++){
        for(int j=0;j < 7;j+=2){
            key[i] = key[i] ^ ( 1<<(i%2+j) );
        }
//        printf("%d,",(unsigned char)key[i]); 
    }
}

/*位取反验证*/
void check_reversebit(){
    for(int i = 0;i < 36;i++){
        if((unsigned char)key[i] != check2_data[i]){
            printf("error.\n");
            exit(0);
        }
    }
}

void flag_num_check2(){
    if(flag_num_merber[19] != flag_num_member_check2[0]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[18] != flag_num_member_check2[1]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[5] != flag_num_member_check2[2]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[7] != flag_num_member_check2[3]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[17] != flag_num_member_check2[4]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[1] != flag_num_member_check2[5]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[0] != flag_num_member_check2[6]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[20] != flag_num_member_check2[7]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[6] != flag_num_member_check2[8]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[29] != flag_num_member_check2[9]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[28] != flag_num_member_check2[10]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[27] != flag_num_member_check2[11]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[15] != flag_num_member_check2[12]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[16] != flag_num_member_check2[13]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[4] != flag_num_member_check2[14]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[3] != flag_num_member_check2[15]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[2] != flag_num_member_check2[16]){
        printf("error\n");
        exit(0);
    }
    if(flag_num_merber[32] != flag_num_member_check2[17]){
        printf("error\n");
        exit(0);
    }
}

void encrpyt2(){
    decryptsmc();
    reversebit();
    check_reversebit();
    flag_num_check2();
}