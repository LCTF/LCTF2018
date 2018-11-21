# b2w WriteUp

## 0x00 WP

项目源码：https://github.com/yeonzi/badappe_oscilloscope/tree/lctf2018

IDA打开：main函数大体注释如下（代码位置src/main.c）

```c
__int64 __fastcall main(__int64 a1, char **a2, char **a3)
{
  __int64 v4; // [rsp+0h] [rbp-100h]
  __int64 v5; // [rsp+8h] [rbp-F8h]
  unsigned int (__fastcall *v6)(unsigned int); // [rsp+10h] [rbp-F0h]
  int v7; // [rsp+98h] [rbp-68h]
  char s; // [rsp+B0h] [rbp-50h]
  unsigned __int64 v9; // [rsp+F8h] [rbp-8h]

  v9 = __readfsqword(0x28u);
  dword_605268 = 45;					//这个是整个程序处理的图片数，下一个for循环用到的
  fwrite("Do Processing Workflow On Each Frame.\n", 1uLL, 0x26uLL, stdout);
  v6 = sub_403539;		//设置SIGALRM信号，其实这个处理函数只是回显当前处理的图片数，对结果没什么影响
  v7 = 0x10000000;
  sigaction(14, (const struct sigaction *)&v6, 0LL);
  alarm(3u);
  v4 = sub_401ADD(2LL, 48000LL, (unsigned int)(2000 * dword_605268));
  						//为一个wav文件分配空间
  for ( dword_60526C = 1; dword_60526C <= dword_605268; ++dword_60526C )
  {
    sprintf(&s, "./flag/%02d_pad.bmp", (unsigned int)dword_60526C, v4);
    fprintf(stdout, "Processing %d frame", (unsigned int)dword_60526C);
    v5 = sub_402806(&s);  //读取一个bmp文件，v5是图片文件的结构体，保存了宽、高、通道数、RGB数据等信息
    if ( !dword_605270 )
    {
      dword_605270 = *(_DWORD *)(v5 + 4);
      dword_605274 = *(_DWORD *)(v5 + 8);
      dword_605278 = 12 * dword_605270 * dword_605274;
    }						//读取图片的宽和高
    fprintf(stdout, "%02d BMP Read Done\n", (unsigned int)dword_60526C);
    sub_400E66(v5);			// 图片二值化 src/image_binary.c
    fwrite("Edge Detecting...\n", 1uLL, 0x12uLL, stdout);
    sub_400F38(v5, 1LL);	// 图片边缘检测 src/edge_detect.c
    sub_400E66(v5);			// 再一次二值化
    fwrite("Path Generating...\n", 1uLL, 0x13uLL, stdout);
    sub_402C7F(v5, v4, (unsigned int)(dword_60526C - 1));
    sub_403878(v5);			//释放图片缓存
  }
  raise(14);
  fwrite("\nProcess end.\n", 1uLL, 0xEuLL, stdout);
  fwrite("\nSaving.\n", 1uLL, 9uLL, stdout);
  sub_401C6D(v4, "./out.wav");	//保存wav文件
  sub_401C16(v4);				//释放wav缓存
  return 0LL;
}
```
其中 400E66二值化的处理过程为：

* 取图片每个像素的三个通道，根据公式 Y = 0.299\*R + 0.587\*G + 0.114\*B 计算，并将Y写回素的三个通道
* 求灰度化后图片的均值
* 大于均值的写入255，小于的写入0

下一步是边缘检测（400F38），我感觉这里printf了一个Edge Detecting很人性化了（逃

边缘检测用到的是canny算子，402C42是一个取图片对应像素的函数，参数2是x，参数3是y

最后一步是生成波形数据，注释如下（代码位置：src/path.c: gen_path）

```c
__int64 __fastcall sub_402C7F(__int64 a1, __int64 a2, int a3) //a1:bmp图片 a2:为波形分配的空间 a3:当前处理的是第几张图片
{
  signed int v3; // ST54_4
  float v4; // xmm2_4
  float v5; // xmm3_4
  int v7; // [rsp+Ch] [rbp-64h]
  int i; // [rsp+28h] [rbp-48h]
  int v9; // [rsp+28h] [rbp-48h]
  int m; // [rsp+2Ch] [rbp-44h]
  int n; // [rsp+2Ch] [rbp-44h]
  int l; // [rsp+30h] [rbp-40h]
  signed int v13; // [rsp+34h] [rbp-3Ch]
  signed int ii; // [rsp+38h] [rbp-38h]
  int v15; // [rsp+3Ch] [rbp-34h]
  int v16; // [rsp+40h] [rbp-30h]
  int v17; // [rsp+44h] [rbp-2Ch]
  int j; // [rsp+48h] [rbp-28h]
  int k; // [rsp+4Ch] [rbp-24h]
  float v20; // [rsp+60h] [rbp-10h]
  float v21; // [rsp+64h] [rbp-Ch]

  v7 = a3;
  v13 = 0;
  if ( !dword_605218 )
  {
    dword_605218 = *(_DWORD *)(a1 + 4) * *(_DWORD *)(a1 + 8);	//605218 = image.width * image.height
    qword_605220 = (__int64)malloc(4LL * dword_605218);
    qword_605228 = (__int64)malloc(4LL * dword_605218);
    qword_605230 = (__int64)malloc(4LL * dword_605218);
    qword_605238 = (__int64)malloc(4LL * dword_605218);
    qword_605240 = (__int64)malloc(4LL * dword_605218);
    qword_605248 = (__int64)malloc(4LL * dword_605218);
    qword_605250 = (__int64)malloc(0x1F40uLL);
    qword_605258 = (__int64)malloc(0x1F40uLL);
  }
  for ( i = dword_605218 - 1; i >= 0; --i )
    *(_DWORD *)(4LL * i + qword_605238) = 0;				//初始化
  v9 = 0;
  for ( j = 1; *(_DWORD *)(a1 + 4) > j; ++j )
  {
    for ( k = 1; *(_DWORD *)(a1 + 8) > k; ++k )
    {
      if ( *(float *)sub_402C42(a1, j, k) > 128.0 )
      {
	*(_DWORD *)(qword_605220 + 4LL * v9) = j;
	*(_DWORD *)(qword_605228 + 4LL * v9) = k;
	*(_DWORD *)(4LL * v9 + qword_605230) = 0x10000;
	*(_DWORD *)(4LL * v9++ + qword_605238) = 1;		//标记数组，用于标记当前点是否已被选取
      }
    }
  }						//遍历图像中每个像素，若该像素大于128（即之前检测到的边缘点）则保存其x,y值
  						//遍历完后v9保存的为图像中边缘点的总数
  for ( l = v9; l > 0; --l )
  {
    for ( m = 0; m < v9; ++m )
    {
      if ( *(_DWORD *)(4LL * m + qword_605238) == 1 )
      {
	v3 = abs(*(_DWORD *)(4LL * m + qword_605220) - dword_605260);
	v4 = (float)(signed int)abs(*(_DWORD *)(4LL * m + qword_605228) - dword_605264);
	v5 = sqrt((float)((float)(v4 * v4) + (float)((float)v3 * (float)v3)));
	*(_DWORD *)(qword_605230 + 4LL * m) = (signed int)v5;
      }
    }					//这段代码对第一个循环中选取的点计算与其他所有点之间的距离保存到605230中
    v16 = *(_DWORD *)(a1 + 4) + *(_DWORD *)(a1 + 8);
    v17 = 0;
    for ( n = 0; n < v9; ++n )
    {
      if ( *(_DWORD *)(4LL * n + qword_605238) == 1 && *(_DWORD *)(4LL * n + qword_605230) < v16 )
      {
	v16 = *(_DWORD *)(4LL * n + qword_605230);
	v17 = n;
      }
    }					//取最小的点
    *(_DWORD *)(qword_605240 + 4LL * v13) = *(_DWORD *)(4LL * v17 + qword_605220);
    *(_DWORD *)(qword_605248 + 4LL * v13) = *(_DWORD *)(4LL * v17 + qword_605228);
    						//605240和605248记录所有已被选取的点，分别记录点在图像中的x和y坐标
    *(_DWORD *)(4LL * v17 + qword_605238) = 0;		//该点已被选取，标记数组置0
    dword_605260 = *(_DWORD *)(4LL * v17 + qword_605220);
    dword_605264 = *(_DWORD *)(4LL * v17 + qword_605228);
    ++v13;
  }
  v20 = (float)v13 / (float)2000;
  v21 = 65535.0 / (float)*(signed int *)(a1 + 4);	//缩放比例，这里的源码被一位师傅指出有误orz
  v15 = 2000 * v7;
  for ( ii = 0; ii < 2000; ++ii )
  {
    *(_WORD *)(**(_QWORD **)(a2 + 16) + 2LL * v15) = (signed int)(float)((float)(*(_DWORD *)(4LL
											   * (signed int)(float)((float)ii * v20)
											   + qword_605240)
									       - *(_DWORD *)(a1 + 4) / 2)
								       * v21);
    *(_WORD *)(*(_QWORD *)(*(_QWORD *)(a2 + 16) + 8LL) + 2LL * v15++) = (signed int)(float)((float)(*(_DWORD *)(4LL * (signed int)(float)((float)ii * v20) + qword_605248)
												  - *(_DWORD *)(a1 + 8) / 2)
											  * v21);
				//将刚刚记录x和y值的数组数据写入wav文件缓存，注意这里分别写入了wav文件的两个声道
				//分别写入的值是x-img.width和y-img.height
  }
  return 0LL;
}
```

可以大概分析出这里的代码将之前图片的边缘点提取出来并分别保存了从某个点开始，每次选取最近点后的结果

最后分析写入wav文件的函数

代码位置：src/wave.c: wave_save.c

```c
signed __int64 __fastcall sub_401C6D(__int64 a1, const char *a2)
{
  signed __int64 result; // rax
  int j; // [rsp+10h] [rbp-A0h]
  int i; // [rsp+14h] [rbp-9Ch]
  int v5; // [rsp+18h] [rbp-98h]
  int v6; // [rsp+20h] [rbp-90h]
  int v7; // [rsp+24h] [rbp-8Ch]
  unsigned __int64 v8; // [rsp+30h] [rbp-80h]
  FILE *s; // [rsp+38h] [rbp-78h]
  int ptr; // [rsp+40h] [rbp-70h]
  int v11; // [rsp+44h] [rbp-6Ch]
  int v12; // [rsp+48h] [rbp-68h]
  int v13; // [rsp+50h] [rbp-60h]
  int v14; // [rsp+54h] [rbp-5Ch]
  __int16 v15; // [rsp+58h] [rbp-58h]
  __int16 v16; // [rsp+5Ah] [rbp-56h]
  int v17; // [rsp+5Ch] [rbp-54h]
  int v18; // [rsp+60h] [rbp-50h]
  __int16 v19; // [rsp+64h] [rbp-4Ch]
  unsigned __int16 v20; // [rsp+66h] [rbp-4Ah]
  char v21[8]; // [rsp+70h] [rbp-40h]
  unsigned __int64 v22; // [rsp+A8h] [rbp-8h]

  v22 = __readfsqword(0x28u);
  strcpy(v21, "LCTF{LcTF_1s_S0Oo0Oo_c0o1_6uT_tH1S_iS_n0t_fL4g}");
  s = fopen(a2, "wb");
  if ( s )
  {
    ptr = 'FFIR';
    v11 = 2 * (*(_DWORD *)(a1 + 8) * *(_DWORD *)a1 + 18);
    v12 = 'EVAW';
    fwrite(&ptr, 0xCuLL, 1uLL, s);
    v13 = ' tmf';
    v14 = 16;
    v15 = 1;
    v16 = *(_DWORD *)a1;
    v17 = *(_DWORD *)(a1 + 4);
    v19 = 4;
    v20 = 16;
    v18 = 2 * v17;
    fwrite(&v13, 0x18uLL, 1uLL, s);
    v6 = 'atad';
    v7 = *(_DWORD *)(a1 + 8) * *(_DWORD *)a1 * (v20 >> 3);
    fwrite(&v6, 8uLL, 1uLL, s);			//写入wav文件的头部信息
    v5 = strlen(v21);
    v8 = 0LL;
    for ( i = 0; *(_DWORD *)(a1 + 8) > i; ++i )
    {
      for ( j = 0; *(_DWORD *)a1 > j; ++j )
      {
	*(_WORD *)(*(_QWORD *)(8LL * j + *(_QWORD *)(a1 + 16)) + 2LL * i) = 257 * v21[v8 % v5] ^ *(_WORD *)(2LL * i + *(_QWORD *)(8LL * j + *(_QWORD *)(a1 + 16)));	
	fwrite((const void *)(*(_QWORD *)(8LL * j + *(_QWORD *)(a1 + 16)) + 2LL * i), 2uLL, 1uLL, s);
		//将刚刚得到的x和y数据与LCTF{LcTF_1s_S0Oo0Oo_c0o1_6uT_tH1S_iS_n0t_fL4g}异或后写入波形文件
	v8 += v21[v8 % v5];
      }
    }
    fclose(s);
    result = 0LL;
  }
  else
  {
    perror("Cannot open wav file");
    result = 0xFFFFFFFFLL;
  }
  return result;
}
```
所以大概可以知道题目所给的wav文件是对记录flag的图片进行边缘检测后提取出轮廓信息，并将轮廓的x和y信息记录到wav文件的两个声道中

到这里可能很多师傅已经猜到这个程序大概可以用来干嘛了，很多人应该看过b站上有一些将音频输出到示波器上播放图片或视频，这程序就是用来做这个的

最后附上keygen，这里的keygen还原图片时有个需要确定的变量就是wav一共包含了几张图片，留意到main函数中主循环一共循环了45次，所以就是45张图

```python
import wave
import math
import numpy as np
import matplotlib.pyplot as pl
from matplotlib import animation
import time
import random

filename = "out"
filename += ".wav"

try:
    fp = wave.open(filename,"rb")
except Exception as err:
        print(err)
        exit(0)

param = fp.getparams()
nchannels,sampwidth,framerate,nframes = param[:4]
print "WAV param"
print "channels = %d, sampwidth = %d, framerate = %d, nframes = %d, compname = %s" % ( nchannels,sampwidth,framerate,nframes,param[4] )
fp_data = fp.readframes(nframes)
fp.close()

wave_data = np.fromstring(fp_data,dtype=np.short)
wave_data.shape = -1, 2

wave_data = wave_data.T
frame = len(wave_data[0])/45

encode = "LCTF{LcTF_1s_S0Oo0Oo_c0o1_6uT_tH1S_iS_n0t_fL4g}"
code_len = len(encode)
tmp = 0
for i in xrange(len(wave_data[0])):
    for j in xrange(2):
        tmp_a = ord(encode[tmp%code_len])
        tmp_b = tmp_a*256 + tmp_a
        #print tmp_b
        wave_data[j][i] ^= tmp_b
        tmp += tmp_a        
n = 0
fig = pl.figure()
pic = fig.add_subplot(111)
line, = pic.plot(wave_data[1][n*frame:(n+1)*frame], wave_data[0][n*frame:(n+1)*frame],marker = ".")
line.set_marker(".")
def animate(i):
    time.sleep(0.5)
    line.set_ydata(wave_data[0][i*frame:(i+1)*frame])
    line.set_xdata(wave_data[1][i*frame:(i+1)*frame])
    return line,
def init():
    pic.set_xlim(-30000,30000)
    pic.set_ylim(-30000,30000)
ani = animation.FuncAnimation(fig=fig, func=animate, init_func = init ,frames=1000)
pl.show()
```
## 0x01 后记

emmm。。。这次LCTF其实是我第一次给一个正式的比赛出题，经验不足也出了很多失误，望各位师傅谅解。各位师傅能做我的题真的很荣幸，也让我学到很多

这道题其实原本是我跟一位学长做示波器播放badapple的源码（大多数代码是学长写的），代码在同项目的master（最早的）和nen9mA0（修了一些bug）两个branch下 

* PS:代码运行比较慢，瞎写的没做什么优化

出题的时候突然有个想法拿这个来出道逆向，因为以前似乎没见过类似的，思路也会比较有趣

本来觉得逆向这一大段程序逻辑本身就是题目难度就不小，因为里面有很多（不明所以的）循环，内存分配还有对图片分辨率和音频码率之类的计算，所以就没有加入太多逆向题常见的混淆/反调/编码之类的东西

最后保存波形进行的异或运算也只是为了防止有哪位师傅脑洞比较大加上之前看过这种操作直接把音频放到软件里得到flag做的一点混淆

不过真的还是有大手子很快就逆出整个逻辑并得到了flag orz，让我自己逆这道应该是做不出来的

此外，**感谢redbud一位大师傅指出了path.c里的一个bug  orz** 

**最后我在这道个歉，这题我出完以后脑子瓦特了没有叫学长把repo关掉 = =，是的有师傅搜字符串搜到了源码orz（虽然是最早的那版。。。lctf的源码我赛后才push的，但原repo主要逻辑都有了），为了公平起见后来也就没有关掉repo了。这是个非常严重的失误，再次跟各位道歉**

~~出题人为了谢罪已经戴上了假发= =（没有女装！）~~