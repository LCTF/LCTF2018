#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
///#include <seccomp.h>
//#include <linux/seccomp.h>

struct node{
    char name[8];
    int* gift;
    int role;
    int level;
}account[10];
struct node hack[2];

char dlll[8];
char card[8];
char e_card[8];





int acc_cnt = 0;
int cur_user = 0;
int e_card_num;
int health = 4;

int card_num;
int e_health;

void secret();
void wel();
void signin();
void signup();
void login();
void checker();
void choose_role();
void liu();
void guan();
void zhang();
void play();
void turn();
void my_turn();

int main(int argc, char const *argv[])
{
    /*scmp_filerter_ctx ctx;
    ctx = seccomp_init(SCMP_ACT_ALLOW);
    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(execve), 0);
    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(execve), 0);
    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(execve), 0);
    seccomp_load(ctx);
    */
    int t;
    strcpy(dlll,"fl4g");
    setbuf(stdin,NULL);
    setbuf(stdout,NULL);
    secret();
    wel();
    login();
    checker();
    choose_role();
    play();
    puts("one more?(0/1)");
    scanf("%d",&t);
    if(t == 1){
        printf("Goodbye! %s",account[cur_user].name);
	login();       
	checker();
        choose_role();
        play();
    }
    return 0;
}

void secret()
{
    char a[] = {"admin"};
    strcpy(account[0].name,a);
    //account[0].gift = &setbuf;
}

void wel()
{
    //puts("L-game: create happiness by money");
    puts("This is just a little game, have fun!");
    puts("press enter to start game");
    char c = getchar();
    if(c!='\n') exit(0);
    return;
}

void signup()
{
    char c = getchar();
    if(acc_cnt > 10){
        puts("too many users!");
        login();
    }
    puts("input your name");
    read(0,account[++acc_cnt].name,8);
    printf("Welcome! %s",account[acc_cnt].name);
    cur_user = acc_cnt;
    return;
}

void signin()
{
    char ss[10];
    char s[8];
    char j;
    char c = getchar();
    s[8] = 1;
    puts("input your name");
    //printf("%d %d\n",s[8],k);
    scanf("%9s",s);
    //printf("s = %s\n",s);
    int i = s[8];
    //printf("i = %d,j = %d\n",i,k);
    for(;i <= acc_cnt; ++i)
        if(!strcmp(s,account[i].name))
        {
            printf("Welcome! %s\n",account[i].name);
            cur_user = i;
            return;
        }
    puts("no such one!");
    login();
    return;
}

void login()
{
    char c;
    while(1)
    {
        puts("sign (I)n or sign (U)p?");
        c = getchar();
        while(c == ' ' || c == '\n') c = getchar();
        if(c != 'I' && c != 'U') puts("invalid choose!");
         else break;
    }
    if(c == 'I') signin();
     else signup();
    return;
}

void checker()
{
    int i;
    char f[5];
    char s[60];
    if(cur_user != 0) return;
    //puts("1");
    f[0] = dlll[0];
    f[1] = dlll[1];
    f[2] = dlll[2];
    f[3] = dlll[3];
    int k = 233;
    f[0] *= k;
    while(k != 240)
    {
        if(f[0] % 2) 
        {
            f[0] = 3 * f[0] + 1;
            k = k * 6;
        }
        else 
        {
            f[0] /= 2;
            k = (k + 39) % 666;
        }
    }
    f[0] += 126;
    //puts("2");
    k = 233;
    f[2] *= k;
    while(k != 144)
    {
        if(f[2] % 2) 
        {
            f[2] = 3 * f[2] + 1;
            k = k * 6;
        }
        else 
        {
            f[2] /= 2;
            k = (k + 39) % 666;
        }
    }
    //printf("%d",f[2]);
    f[2] = (-45 * f[2] + 865)/13;
    //printf("%d",f[2]);
    k = 233;
    f[3] *= k;
    while(k != 240)
    {
        if(f[3] % 2) 
        {
            f[3] = 3 * f[3] + 1;
            k = k * 6;
        }
        else 
        {
            f[3] /= 2;
            k = (k + 39) % 666;
        }
    }
    f[3] += 102;


    int fd = open(f,O_RDONLY);

    read(fd,s,60);
    puts("congrantualtions!");
    printf("%s\n",s);
}

void liu()
{
    int i;
    srand(time(NULL));
    health = 7;
    e_card_num = 4;
    for(i = 0;i <= 3; ++i)
        {
            int t = rand() % 3;
            if(t == 0) e_card[i] = 'a';
            if(t == 1) e_card[i] = 'g';
            if(t == 2) e_card[i] = 'p';
        }
    
    e_health = 7;
    card_num = 5;
    for(i = 0;i <= 4; ++i)
        {
            int t = rand() % 3;
            if(t == 0) card[i] = 'a';
            if(t == 1) card[i] = 'g';
            if(t == 2) card[i] = 'p';
        }
    return;
}

void guan()
{
    int i;
    srand(time(NULL));
    health = 3;
    e_card_num = 4;
    for(i = 0;i <= 3; ++i)
        {
            int t = rand() % 3;
            if(t == 0) e_card[i] = 'a';
            if(t == 1) e_card[i] = 'g';
            if(t == 2) e_card[i] = 'p';
        }
    
    e_health = 5;
    card_num = 4;
    for(i = 0;i <= 3; ++i)
        {
            int t = rand() % 3;
            if(t == 0) card[i] = 'a';
            if(t == 1) card[i] = 'g';
            if(t == 2) card[i] = 'p';
        }
       
    return;
}
void zhang()
{
    int i;
    srand(time(NULL));
    health = 2;
    e_card_num = 4;
    for(i = 0;i <= 3; ++i)
        {
            int t = rand() % 3;
            if(t == 0) e_card[i] = 'a';
            if(t == 1) e_card[i] = 'g';
            if(t == 2) e_card[i] = 'p';
        }
    
    e_health = 5;
    card_num = 4;
    for(i = 0;i <= 3; ++i)
        {
            int t = rand() % 3;
            if(t == 0) card[i] = 'a';
            if(t == 1) card[i] = 'g';
            if(t == 2) card[i] = 'p';
        }
       
    return;
}

void choose_role()
{
    int c;
    puts("game start!");
    puts("you have ida, you know!");
    puts("1. do something"); //add
    puts("2. do something again"); //del
    puts("3. do something else"); //edi
    puts("4. do nothing");
    scanf("%d",&c);
    switch (c)
    {
        case 1:
            puts("you choose 1");
            liu();
            break;
        case 2:
            puts("you choose 2");
            guan();
            break;
        case 3:
            puts("you choose 3");
            zhang();
            break;
        case 4:
            puts("goodbye!"); 
            exit(-1);
            break;
        default:
            puts("invalid choose!");
            fflush(stdin);
            choose_role();
            break;
    }
    return;
}

void check_health(int n)
{
    int i;
    int flag = 0;
    if(n == 1)
    {
        if(e_health > 0) return;
        for(i = 0;i < card_num; ++i)
        if(card[i] == 'p')
        {
            card_num--;
            for(;i < card_num; ++i) card[i] = card[i+1];
            puts("COM: eat a peach and +1 health");
            e_health++;
        }  
    }
    else
    {
        if(health > 0) return;
        for(i = 0;i < e_card_num; ++i)
            if(e_card[i] == 'p')
            {
                flag = 1;
                break;
            }
        if(!flag)
        {
            puts("you don't have a peach e_card! GG!");
            return;
        }
        e_card_num--;
        for(;i < e_card_num; ++i) e_card[i] = e_card[i + 1];
        printf("%s: eat a peach and +1 health\n",account[cur_user].name);
        health++;
        return;
    }
} 

void attack()
{
    int i;
    int flag = 0;
    for(i = 0;i < e_card_num; ++i)
        if(e_card[i] == 'a')
        {
            flag = 1;
            break;
        }
    if(!flag)
    {
        puts("you don't have a attack e_card!");
        return;
    }
    e_card_num--;
    for(;i < e_card_num; ++i) e_card[i] = e_card[i + 1];
    printf("%s: Attack!\n",account[cur_user].name);
    flag = 0;
    for(i = 0;i < card_num; ++i)
    if(card[i] == 'g')
        {
            flag = 1;
            break;
        }
    if(!flag)
    {
        puts("COM: -1 health");
        e_health--;
        if(e_health == 0) 
        check_health(1);
        return;
    }
    else{
            card_num--;
            for(;i < e_card_num; ++i) e_card[i] = e_card[i + 1];
            puts("COM: Guard!");
        }
    return;
}

void eat()
{
    if(health > 6)
    {
        puts("you can't eat more peaches!");
        return;
    }
    int i;
    int flag = 0;
    for(i = 0;i < e_card_num; ++i)
        if(e_card[i] == 'p')
        {
            flag = 1;
            break;
        }
    if(!flag)
    {
        puts("you don't have a peach e_card!");
        return;
    }
    e_card_num--;
    for(;i < e_card_num; ++i) e_card[i] = e_card[i + 1];
    printf("%s: eat a peach and +1 health\n",account[cur_user].name);
    health++;
    return;
}

void turn()
{
    int c;
    puts("it's your turn, draw!");
    int t = rand() % 3;
    if(t == 0) e_card[e_card_num++] = 'a';
    if(t == 1) e_card[e_card_num++] = 'g';
    if(t == 2) e_card[e_card_num++] = 'p';
    //e_card[e_card_num++] = rand() % 3 + 1;
    //e_card[e_card_num++] = rand() % 3 + 1;
    puts("this is your e_cards");
    int cnt = 0;
    while(cnt < e_card_num)
        if(e_card[cnt] == 'a') printf("%d Attack ",++cnt);
        else if(e_card[cnt] == 'g') printf("%d Guard ",++cnt);
        else if(e_card[cnt] == 'p') printf("%d Peach ",++cnt);
    puts("");
    printf("your health is %d\n",health);
    printf("enemy e_cards: %d enemy health: %d\n",card_num,e_health);
    puts("what to do now");
    puts("!!!you can only try to do one thing in one turn!!!");
    puts("1. Attack");
    puts("2. Eat Peach");
    puts("3. Pass");
    while(1)
    {
        scanf("%d",&c);
        if(c == 1)
        {
            attack();
            break;
        }
        else if(c == 2)
        {
            eat();
            break;
        }
        else if(c == 3) break;
        else puts("invalid choose!");
    }
    return;
}

void throw()
{
    int cnt = 0;
    int i,j;
    if(e_card_num <= health) return;
    puts("you have to throw you e_card!");
    int tmp = e_card_num - health;
    while(1)
    {
        while(cnt < e_card_num)
            if(e_card[cnt] == 'a') printf("%d Attack ",++cnt);
            else if(e_card[cnt] == 'g') printf("%d Guard ",++cnt);
            else if(e_card[cnt] == 'p') printf("%d Peach ",++cnt);
        puts("");
        puts("put the e_card number you want to throw");
        scanf("%d",&j);
        if(j > e_card_num || j < 0) puts("invalid input");
            else
            {
                e_card_num--;
                for(;j <= e_card_num; ++j) e_card[j-1] = e_card[j];
                break;
            }
    }
    cnt = 0;
    if(tmp > 1)
    {
         while(1)
        {
            while(cnt < e_card_num)
                if(e_card[cnt] == 'a') printf("%d Attack ",++cnt);
                else if(e_card[cnt] == 'g') printf("%d Guard ",++cnt);
                else if(e_card[cnt] == 'p') printf("%d Peach ",++cnt);
            puts("");
            puts("put the e_card number you want to throw");
            scanf("%d",&j);
            if(j > e_card_num) puts("invalid input");
                else
                {
                    e_card_num--;
                    for(;j <= e_card_num; ++j) e_card[j-1] = e_card[j];
                    break;
                }
        } 
    }

    return;
}
void my_turn()
{
    int i,j,cc;
    int flag = 0;
    //puts("**************************");
    puts("it's my turn, draw!");
    int t = rand() % 3;
    if(t == 0) card[card_num++] = 'a';
    if(t == 1) card[card_num++] = 'g';
    if(t == 2) card[card_num++] = 'p';
    //card[card_num++] = rand() % 3 + 1;
    if(e_health < 5)
    {
        for(i = 0;i < card_num; ++i)
        if(card[i] == 'p')
        {
            card_num--;
            for(;i < card_num; ++i) card[i] = card[i+1];
            puts("COM: eat a peach and +1 health");
            e_health++;
            break;
        }
    }
    for(i = 0;i < card_num; ++i)
        if(card[i] == 'a')
        {
            card_num--;
            for(;i < card_num; ++i) card[i] = card[i+1];
            puts("COM: Attack!");
            for(j = 0;j < e_card_num; ++j)
            {
                if(e_card[j] == 'g')
                {
                    puts("do you want to guard?[0/1]");
                    scanf("%d",&cc);
                    if(cc == 1) flag = 1;
                    break;
                }
            }
            if(!flag)
            {
                health--;
                printf("%s: -1 health\n",account[cur_user].name);
                check_health(2);
            }
            else{
                e_card_num--;
                for(;j < e_card_num; ++j) e_card[j] = e_card[j+1];
                printf("%s: Guard!\n",account[cur_user].name); 
            }
            break;
        }
    if(card_num > e_health) card_num = e_health;
    return;
}
void play()
{
    int turn_num = 0;
    char t[50];
    while(health > 0 || e_health < 0)
    {
        printf("-------turn %d-------\n",++turn_num);
        turn();
        throw();
        puts("-------your turn is over-------");
        my_turn();
    }
    if(health > 0) puts("you win!");
        else puts("you lose!");
    if(health > 0){
        puts("put the words you want to talk");
        scanf("%4s",t);
        printf(t);
    }
   // checker();
}
