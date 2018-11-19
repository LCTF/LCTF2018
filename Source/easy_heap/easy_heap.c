#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
struct pt
{
	char * pt1;
	unsigned int size;
};
struct cola
{
	pt kele[10];
};
cola *tk;
void init()
{
	setvbuf(stdout,0LL,2,0LL);
	setvbuf(stdin,0LL,1,0LL);
	setvbuf(stderr, 0LL, 2, 0LL);
	alarm(0x3c);
	puts("_____        ____________________________  ");
	puts("|===|        |========|=========|=======|");
	puts("|===|        |===|￣￣   |===|  |==|￣￣ ");
	puts("|===|        |===|       |===|  |==|____  ");
	puts("|===|        |===|       |===|  |==|===| ");
	puts("|===|        |===|       |===|  |==|￣￣   ");
	puts("|===|________|===|____   |===|  |==|      ");
	puts("|============|========|  |===|  |==|  ");
	puts("￣￣￣￣￣￣￣￣￣￣￣   ￣￣   ￣￣   ");
}
void menu()
{
	puts("------------------------");
	puts("1: malloc. ");
	puts("2: free. ");
	puts("3: puts. ");
	puts("4: exit. ");
	puts("------------------------");
	printf("which command?\n> ");
}
void bye_bye()
{
	puts("bye bye~!");
	exit(0);
}

void safe_read (char *pt1,unsigned int size)
{
	int i=0;
	if(size==0)
	{
		pt1[0]='\0';
		return;
	}	
	while(1)
	{
		read(0,&pt1[i],1);
		if(i>size-1 || pt1[i]=='\0' || pt1[i]=='\n')
			break;
		i+=1;
	}
	pt1[i]='\0';
	pt1[size]='\0';
}
int get_atoi()
{
	char buf[4];
	read(0,buf,4);	
	return atoi(buf);
}
void malloc_1()
{
	int i;
	unsigned int size;
	for (i=0;i<=9;i++)
	{
		if ( tk->kele[i].pt1==NULL)
			break;
		else continue;
	}
	if (i==10)
		{
			puts("full!");
			return;
		}	
	tk->kele[i].pt1=(char *)malloc(0xf8);
	if (tk->kele[i].pt1==NULL)
		{
			puts("malloc error!");
			bye_bye();
		}
	printf("size \n> ");
	size=get_atoi();
	if(size>0xf8)
		bye_bye();
	tk->kele[i].size=size;
	printf("content \n> ");
	safe_read(tk->kele[i].pt1,tk->kele[i].size);
}
void delete_1()
{
	unsigned int index;
	printf("index \n> ");
	index=get_atoi();
	if(index>=10 || tk->kele[index].pt1==NULL)
		bye_bye();
	memset(tk->kele[index].pt1,'\0',tk->kele[index].size);
	free(tk->kele[index].pt1);
	tk->kele[index].size=0;
	tk->kele[index].pt1=NULL; 
}
void puts_1()
{
	unsigned int index;
	printf("index \n> ");
	index=get_atoi();
	if(index>=10 || tk->kele[index].pt1==NULL)
		bye_bye();
	puts(tk->kele[index].pt1);
}
int main(void)
{
	init();
	tk= (cola*) calloc(0xa0,1);
	if(tk==NULL)
	{
		puts("init error!");
		bye_bye();
	}
	while(1)
	{
		int choose;
		menu();
		choose=get_atoi();
		switch(choose)
		{
			case 1:
				malloc_1();
				break;
			case 2:
				delete_1();
				break;
			case 3:
				puts_1();
				break;
			case 4:
				bye_bye();
				break;
			default:
				break;
		}
	}
	return 0;
}
	
