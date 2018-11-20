#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
#include<unistd.h>
#include <time.h>
#define PART_NUM 16
#define MAX_PART 5
struct MESSAGE{
    char title[32];
    char *content;
};

int identify();
int welcome();
int buybuybuy();
int getProfessional();
int leaveMessage(int len, struct MESSAGE messages[]);
int getflag();

char *CBCEncrypt(const char *plaintext);
char *Keyfunc(const char *key, char *part);
char *InvKeyfunc(const char *key, char *part);
char *CBCDecrypt(const char *iv, const char *encryption);
char *NumberToHex(const char *numberArray, int len);
char *HexToNumber(const char *hexText);

int s_read(char *buff, size_t size, char end);
int readNumber();
int CheckKeyWord(const char * keyword);
int my_gets(int len, char *buf);

int money;
int message_num;
int try_times;
char randchar[PART_NUM];
int xorIndex1[PART_NUM];
int xorIndex2[PART_NUM];
char xorNum[PART_NUM];

int main()
{
    int flag = 0, choice = 0;
    char plaintext[] = "guest_account:0004;guestname:user";
    char *CipherText;
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    srand(time(0));
    for(int i = 0; i < PART_NUM; i++)
    {
        randchar[i] = rand() % 256;
        xorIndex1[i] = rand() % PART_NUM;
        xorIndex2[i] = rand() % PART_NUM;
        xorNum[i] = rand() % 256;
    }
    while(1)
    {
        printf("You need to sign in or sign up.\n");
        printf("1.Sign up\n");
        printf("2.Sign in\n");
        printf("3.Exit\n");
        choice = readNumber();
        switch(choice)
        {
            case 1:
                    if(flag)
                    {
                        puts("Your've already signed up.");
                    }
                    else
                    {
                        CipherText = CBCEncrypt(plaintext);
                        printf("These is your secretcode:\n");
                        printf("iv=31323334353637386162636465666768;CipherLen=0096;CipherText=%s;\n", CipherText);
                        free(CipherText);
                        flag = 1;
                    }
                break;
            case 2:
                goto outer;
                break;
            case 3:
                exit(0);
            default:
                printf("error input\n");
                break;
        }
    }
outer:
    flag = 0;
    while(1)
    {
        flag = identify();
        while(getchar()!='\n');
        if(flag == 1)
        {
            printf("Welcome!\n");
            welcome();
            break;
        }
        else if(flag == 2)
            printf("No access.\n");
        else if(flag == 3)
            printf("Your secret code is broken.\n");
    }
    
    return 0;
}

int identify()
{
    try_times++;
    if(try_times> 255 * 49)
        exit(0);
    
    char ch, iv[PART_NUM*2+1], number[5], *CipherText, *decryption;
    int len = 0;

    printf("Enter your secret code please:\n");

    if(CheckKeyWord("iv=")) return 3; 
    if(my_gets(PART_NUM*2, iv)) return 3;
    iv[PART_NUM*2] = '\0';    

    if(CheckKeyWord("CipherLen=")) return 3;
    if(my_gets(4, number)) return 3;
    number[4] = '\0';
    
    len = atoi(number);
    CipherText = (char*)malloc(len+1);
    if(len <= 0 || len % 2 || len > PART_NUM * MAX_PART * 2) return 3;

    if(CheckKeyWord("CipherText=")) return 3;
    if(my_gets(len, CipherText)) return 3;
    CipherText[len] = 0;
    
    decryption = CBCDecrypt(iv, CipherText);
    free(CipherText);
    
    if(decryption == NULL)
        return 3;
    
    if(memcmp(decryption, "guest_account:", 14)) 
        return 2;
    memcpy(number, &decryption[14], 4);
    if (atoi(number) < 0)
        return 2;

    if(strcmp(&decryption[29], "user"))
        return 2;
        	    
    memcpy(number, &decryption[14], 4);
    money = atoi(number);
    
    return 1;
}

int getflag()
{
	system("/bin/sh");
}

int welcome()
{
	struct MESSAGE messages[10];
    int64_t  choice2 = 0 ,len = 0,choice3 = 0,choice = 0;
    printf("hello:user!Your account:%d.", money);
    while(1)
    {
        puts("\n\nWhat do you like to do?");
        puts("------------------------");
        puts("1.push message");
        puts("2.feedback");
        puts("3.buy professional eddtion");
        puts("4.hit on the head of the developer");
        puts("------------------------");
        choice = readNumber();
        switch(choice)
        {
            case 1:
            	if(money < 1111)
				{
					puts("You don't have enough money\n;");
					break;
				}
                if(message_num >= 10)
                {
                    puts("You can't leave more than 10 messages, please buy professional edition,hahaha");
                    break;
                }
                puts("How long is your message?");
		        len = readNumber();
                if(len < 0 || len > 0x20)
                {
                    puts("Length is out of range!");
                    break;
                }
                leaveMessage(len,messages);
                money -= 4444;
                break;
            case 2:
				puts("You can't give any opinion in free edition");			
                break;
            case 3:
            	if(money <= 1111)
				{
					puts("It seems you are so poor\n;");
					break;
				}
            	buybuybuy();
				puts("thanks for your money!");
            	getProfessional();
            	money -= 4444;
            	break;
            case 4:
            	puts("no door!\n");
            	exit(0);
            	break;
            default:
                puts("wrong choice!");
                break;
        }
    }
}

int leaveMessage(int len, struct MESSAGE messages[])
{
	char buf[0x20];
    char choice[2];
    int length;
    puts("Enter the content of your message:");
    messages[++message_num].content = malloc(len);
    len = read(0, buf, len-1);
    memcpy(messages[message_num].content, buf, len);
    messages[message_num].content[len] = '\0';
    
    puts("Enter the title of your message:");
    read(0, messages[message_num].title, 50);
    
    puts("Attention, you can't withdraw or change it in free edition.");
    puts("Confirm your pushing? y to confirm.");
    read(0, choice, 2);
    if(choice[0] != 'y')
    {
        free(messages[message_num].content);
        message_num--;
    	puts("Would you like to change? y to confirm.");
    	read(0, choice, 2);
		if(choice[0] == 'y')
		{
            puts("How long is your message?");
	        length = readNumber();
            if(length < 0 || length > 0x80)
            {
                puts("Length is out of range!");
				return 0;
            }
            messages[++message_num].content = malloc(length);
    		puts("Enter the content of your message:");
    		read(0, messages[message_num].content, length);
		    puts("Enter the title of your message:");
    		read(0, messages[message_num].title, 32);
		    
		}			
    }
    
    return 0;  
}


int buybuybuy()
{
	char ch[0x10];
	puts("Confirm? y to confirm");
	read(0, ch, 0x2);
	if(ch[0] != 'y')
	{
		puts("No! I think you should input y!");
		buybuybuy();
	}
	return 0;
}

int getProfessional()
{
	char ch[0xc0];
	puts("thanks for your money!\n");
	puts("tell me why do want to buy my software:");
	read(0, ch, 0x200);
	printf("So your reason is:\n%s", ch);
	puts("But I think your reason is not good.");
	puts("So I don't want to give you the professional edtion.");
	puts("But for some reason, your money can't come back....I'm really sorry.");
	return 0;
}
int s_read(char *buff, size_t size, char end)
{
    int len;
    char ch;
    for(len = 0; len < size; len++)
    {
        read(0, &ch, 1);
        if(end && ch == end)
            break;
        else
            buff[len] = ch;
    }
    return len;
}

int readNumber()
{
    char buff[100], ch;
    int len, i = 0;
    buff[len = s_read(buff, 99, '\n')] = '\0';
    if(len == 99)
        while(read(0,&ch,1))
            if(ch == '\n')
                break;
    return atoi(buff);
}

int CheckKeyWord(const char * keyword)
{
    char buf[100];
    int len = strlen(keyword);
    s_read(buf, len, 0);
    buf[len] = 0;
    return strcmp(buf, keyword);
}

int my_gets(int len, char *buf)
{
    char ch;
    int readed = 0;
    readed = s_read(buf, len, 0);
    read(0, &ch, 1);
    if(ch != ';' || readed != len) return 1;
    return 0;
}

char *CBCEncrypt(const char *plaintext)
{
    int i = 0, j = 0, len = 0;
    char *randKey = randchar;
    char IV[] = "12345678abcdefgh";
    char *encryption, *ret;
    
    //copy
    while(plaintext[i++]) len++;
    if(len > MAX_PART * PART_NUM)
        exit(-1);
    encryption = (char*)malloc(MAX_PART * PART_NUM);
    memcpy(encryption, plaintext, len);
    
    //padding
    if(len % PART_NUM)
    {
        for(i = 0; (len+i) % PART_NUM != 0; i++)
            encryption[len+i] = PART_NUM-len % PART_NUM;
        len = len + i;
    }
    else
    {
        for(i = 0; i < PART_NUM; i++)
            encryption[len+i] = PART_NUM;
        len = len + PART_NUM;
    }
 
    //encrypt
    for(i = 0, j = PART_NUM; i * PART_NUM < len; i++, j = PART_NUM)
    {
        while(j--) encryption[i * PART_NUM + j] ^= (i ? encryption[(i-1) * PART_NUM + j] : IV[j]);
        Keyfunc(randKey, &encryption[i * PART_NUM]);
    }
    
    ret = NumberToHex(encryption,len);
    free(encryption);
    return ret;
}

char *CBCDecrypt(const char *iv, const char *encryption)
{
    int i = 0, j = 0, len = 0, padding = 0;
    char *randKey = randchar;
    char *IV = HexToNumber(iv);
    char *decryption, *copy, *temp;

    len = strlen(encryption)/2;
    temp = HexToNumber(encryption);
    if(temp == NULL) return NULL;
    copy = (char*)malloc(len);
    decryption = (char*)malloc(len+1);
    memcpy(copy, temp, len);
    memcpy(decryption, temp, len);
    free(temp);    
    
    for(i = 0, j = PART_NUM; i * PART_NUM < len; i++, j = PART_NUM)
    {
        InvKeyfunc(randKey, &decryption[i * PART_NUM]);
        while(j--) decryption[i * PART_NUM + j] ^= (i ? copy[(i-1) * PART_NUM + j] : IV[j]);
    }
    free(copy);
    free(IV);
    padding = decryption[(--i)*PART_NUM + PART_NUM -1];
    if(padding > 0 && padding <= PART_NUM)
    {
        for(j = 0; j < padding; j++)
            if(decryption[i*PART_NUM + PART_NUM -1 - j] != padding)
            {
                free(decryption);
                return NULL;
            }
        decryption[i*PART_NUM + PART_NUM - j] = '\0';
    }
    else
    {
        free(decryption);
        return NULL;
    }
    return decryption;
}

char *Keyfunc(const char *key, char *part)
{
    for(int i = 0; i < PART_NUM; i++)
        part[i] += key[i];
    for(int i = 0; i < PART_NUM; i++)
    {
        part[xorIndex1[i]] ^= xorNum[i];
        part[xorIndex2[i]] ^= xorNum[i];
    }
}

char *InvKeyfunc(const char *key, char *part)
{
    for(int i = 0; i < PART_NUM; i++)
    {
        part[xorIndex1[i]] ^= xorNum[i];
        part[xorIndex2[i]] ^= xorNum[i];
    }
    for(int i = 0; i < PART_NUM; i++)
        part[i] -= key[i];
}

char *NumberToHex(const char *numberArray, int len)
{
    char *convertTable = "0123456789ABCDEF";
    char *hexText = malloc(len*2+1);
    for(int i = 0; i < len; i++)
    {
        hexText[2*i] = convertTable[ ((unsigned char)numberArray[i]) >> 4];
        hexText[2*i+1] = convertTable[numberArray[i] & 15];
    }
    hexText[len*2] = '\0';
    return hexText;
}

char *HexToNumber(const char *hexText)
{
    int len = strlen(hexText), low, high;
    char *numberArray = malloc(len/2);
    if(len < 0 || len % 2 == 1) 
    {
        free(numberArray);
        return NULL;
    }
    for(int i = 0; i < len; i++)
        if(hexText[i] < '0' || 
            (hexText[i]>'9' && hexText[i] < 'A') ||
            (hexText[i]>'F') )
        {
            free(numberArray);
            return NULL;
        }
    
    for(int i = 0; i < len/2; i++)
    {
        high = (hexText[2*i] <= '9')?(hexText[2*i]-'0'):(hexText[2*i]-'A'+10);
        low = (hexText[2*i+1] <= '9')?(hexText[2*i+1]-'0'):(hexText[2*i+1]-'A'+10);
        numberArray[i] = low + high * 16;
    }
    return numberArray;
}