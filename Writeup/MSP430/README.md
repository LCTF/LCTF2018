# MSP430 WriteUp

## 0x00 WP

项目源码：https://github.com/LCTF/LCTF2018/tree/master/Source/MSP430

这题主要思想就是实现一个类似于硬件加密器的东西，MSP430板子通过串口接收需要加密的字串，进行RC4加密后通过串口返回结果。

不知道为什么用IDA 7.0载入.out文件会有解析问题，用6.8载入虽然报错但可以看到代码

题目保留了符号表，因此可以大概确定是使用RC4对输入进行加密，密钥生成是用keygen函数进行的

压缩包中给了一张接线图，其实原本是因为RC4的密钥生成代码如下
```c
char key[]="LCTF0000";
int keygen(char *key)
{
	unsigned char tmp = 0;
	P2DIR &= ~(BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5);
	P2REN |= (BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5);
	tmp = P2IN;

	key[4] = tmp * 3;
    key[5] = tmp << 1;
    key[6] = tmp & 0x74;
    key[7] = tmp + 0x50;
}
```
tmp是从芯片的P2 IO口读入IO口的电平，以此为种子做一些运算生成密钥的后四位

图片中可以看到P2.0、P2.4和P2.3接了GND（sorry我发现P2.0那条线接GND还是VCC没拍清楚 = =）

所以tmp的值是 0010 0110 （P2DIR和P2REN中设置了P2的0~5位为输入，默认高电平）

不过其实大体框架逆完后这个字节是可以直接爆破的

爆破脚本如下，来自Aurora战队：


```python
from Crypto.Cipher import ARC4

cipher ="2db7b1a0bda4772d11f04412e96e037c370be773cd982cb03bc1eade".decode("hex")
for i in xrange(0x100):
    k4 = (i * 3) & 0xFF
    k5 = (i * 2) & 0xFF
    k6 = ((i & 0x74) * 2) & 0xFF
    k7 = (i + 0x50) & 0xFF
    key = "LCTF" + chr(k4) + chr(k5) + chr(k6) + chr(k7)
    arc4 = ARC4.new(key)
    plain = arc4.decrypt(cipher)
    if(plain.find("CTF") != -1):
        print(plain)
        
```
当然四位全爆破也是没问题的



## 0x01 后记

这题其实一开始没打算写的，但现在很多ctf都会有一题不同架构硬件的逆向，刚好以前也做过一点相关的开发，就出了一题。MSP430这个系列的芯片在现实中应用还是比较广的，但ctf似乎很少出过。

很多师傅都感觉有点简单了，主要还是因为压缩包里的.out文件有符号表

当时感觉题放的比较晚而且还是个在ctf中比较陌生的架构，因此我没把符号表去掉

结果被师傅们刷爆了 orz

其实这题出的时候还是有很多失误的。。。比如写之前没有拖到IDA里看看，写完才发现这个.out文件在IDA 7.0是会有解析错误的，在IDA 6.8虽然能打开，但还是会报错，包括该款芯片的cfg文件不管是IDA目录下还是github我都没找到

其实原本是想只给一个hex文件和两张图，但考虑到时间，还有CCS编译完后的hex似乎加了一些地址信息导致不能直接用hex2bin转换固件，因此最后给了.out文件

最后，出题人打算这两天看看载入MSP430 hex文件到IDA的正确方法，写完了还更到这个repo（咕咕咕警告）