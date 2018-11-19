//LCTF{this-RevlrSE=^V1@Y+)fAxyzXZ234}
//编译参数：gcc src.c src2.c des.c -masm=intel -std=c99 -s -o easyre
#include<stdio.h>
#include<string.h>
#include <windows.h>
#include <stdbool.h>
#include <tlhelp32.h>
#include "src.h"
#include "des_ecb_enc.h"
struct node Tree[150];
int size = 0;
int count2 = 0;
int flag_num = 0;
int flag_num_merber[36]; 
char key[40];
unsigned char des[50];
int mat[50];
char *reversebit_adr = reversebit;
int mat_enc[36]={23,65,24,78,43,56,
				59,67,21,43,45,76,
				23,54,76,12,65,43,
				89,40,32,67,73,57,
				23,45,31,54,31,52,
				13,24,54,65,34,24};
int mat_check[36] = {43666,49158,43029,51488,53397,51921,
				28676,39740,26785,41665,35675,40629,
				32311,31394,20373,41796,33452,35840,
				17195,29175,29485,28278,28833,28468,
				46181,58369,44855,56018,57225,60666,
				25981,26680,24526,38780,29172,30110};
const char* DebuggerExe[] = {"idaq","OllyDbg","x32dbg","x64dbg","x96dbg","ida","windbg","OD.exe"};
int Debugcheck(){
	int beingdebug = 0;
	__asm__("mov eax, dword ptr fs:[0x30]\n");
	__asm__("movzx eax, byte ptr ds:[eax + 0x2]\n");
	__asm__("mov dword ptr [ebp-4], eax\n");
	if(beingdebug==1){
		exit(0);
	}
//	printf("%d",beingdebug);
	return decryptsmc2();
}

void Debugcheck2(){
	PROCESSENTRY32 myfile;
	myfile.dwSize = sizeof(myfile);
	HANDLE EveryProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//获取所有进程
	int next = Process32First(EveryProcess,&myfile);	//获取进程句柄
	while(next){
		for(int i = 0;i < strlen(myfile.szExeFile);i++){
			tolower(myfile.szExeFile[i]);
		}
//		printf("%s\n",myfile.szExeFile);
		for(int i=0;i < sizeof(DebuggerExe)/4;i++){
			if(strstr(myfile.szExeFile,DebuggerExe[i])){exit(0);}
		}
		next = Process32Next(EveryProcess,&myfile);
	}
}
//__attribute((constructor))指定函数在main函数之前执行
__attribute((constructor)) void Check_Whether_Beingdebug(){
    Debugcheck();
	Debugcheck2();
}
int decryptsmc2(){
	DWORD flOldProtect;
	char *Check_Whether_Beingdebug_addr = Check_Whether_Beingdebug;//18
	char *Debugcheck_addr = Debugcheck;//51
	char *Debugcheck2_addr = Debugcheck2;//284
	int j = 0;
	VirtualProtect(reversebit,1, 0x40u, &flOldProtect);//设置内存可读可写可执行
	for(int i = 0;i < 69;i++){
		if(i<18){
			reversebit_adr[i] ^= Check_Whether_Beingdebug_addr[i];
		}else{
        	reversebit_adr[i] ^= Debugcheck_addr[j];
			j++;
    	}
    }
	for(int i = 0;i < 119;i++){
        reversebit_adr[i] ^= Debugcheck2_addr[i];
    }
	for(int i = 0;i < 119;i++){
        reversebit_adr[i] ^= Debugcheck2_addr[i+119];
    }
	VirtualProtect(reversebit,1, &flOldProtect, NULL);//恢复
}
int main()
{
    FILE *p;
    char flag[40];
    memset(flag_num_merber,0,36*sizeof(int));
    int count;
	printf("Welcome to LCTF2018.\n");
	printf("This is the simplest reverse problem.\n");
	printf("Please enter the flag:");
    scanf("%s",flag);
    if(strlen(flag)!=36){
		printf("error\n");
		exit(0);
	}

    Node tree=NULL;
    for(int i=0;i<strlen(flag);i++){
            tree=build(tree,flag[i]);
            count++;
    }
    count2 = 0;
    first(tree);
//    printf("%s\n",key);
    memset(des,0,strlen(des)*sizeof(char));
	encrpyt1(key);

    count2 = 0;
    last(tree);
    encrpyt2();

    printf("congratulation.\n");
    return 0;
}

