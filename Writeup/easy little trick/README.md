[TOC]

# easy little trick write up

## 题目

>0x774Fea9014010a62017C739EAcB760D8E9B40B75 ROPSTEN 
>
>function level1(?){?} 
>function level2(?){?} 
>function flag(string b64email)public payable { 
>require(pass2[msg.sender] && pass1[msg.sender]); 
>emit GetFlag(b64email, "Get flag!"); 
>}

## 分析

通过2关即可到的flag，逆向出逻辑，然后想办法满足条件就行。[题目地址(比赛结束已经开源了)](https://ropsten.etherscan.io/address/0x774Fea9014010a62017C739EAcB760D8E9B40B75#code  "感谢ROPSTEN的大力支持233")  [在线反编译器](https://ethervm.io/decompile)

## level1

三个输入,很容易判断类型。

很容易看出要满足的条件是

1. `block.blockHash(arg2)`需要小于10
2. `block.blockHash(block.number)`(其实就是0)需要和`arg1`相等
3. `msg.sender`的代码长度为0


随便部署个合约，在`constructor`里调用一下leve1，很简单第一关就过了

   ## level2

结合的看一下，总结:

1. `arg1`为地址的合约代码长度必须为9个字节

2. 调用合约，返回`block.difficulty`的值

9个字节比较苛刻，所以就得构造一下合约.

>初始化opcode:
>60 09 // runtime bytecode 长度
>60 0c // runtime opcodes 位置
>60 00 // 目的内存地址，设为 0 其他也行
>39    // 复制到内存中
>60 09 // runtime bytecode 长度
>60 00 // 内存地址为 0
>f3    // 返回到EVM

> 运行opcode
> 44    // 栈顶为 difficulty
> 60 00 // 00 20 40都行
> 52    // difficulty放到内存
> 60 20 // uint256 为 32bytes 长
> 60 00 // 在内存中的地址
> f3    // 返回

构造好之后为opcode为`6009600c60003960096000f34460005260206000f3`，发送交易创建合约，然后把合约的地址传进去就行了。

当然，还有另一种方法，由于`var3 = var1 & 0xff == 0x09;`可见这里只用了一个字节，所以溢出一下长度，也是可以达到需要的条件。

有啥不对的烦请师傅们批评指正。


