import random
import string
import base64
import datetime
from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad

key = b"lctf2018lctf2018"
block_size = 16

def random_str(length=5):
    random.seed(None)
    return ''.join(random.choice(string.ascii_letters + string.digits) for _ in range(length))

def get_username():
    username = random_str(length=5)
    return username if username != "admin" else get_username()

def check_token(token):
    if token == "" or token is None:
        return False
    try:
        token = str.replace(token, " ", "+")
        token = base64.b64decode(token)
        cipher = AES.new(key, AES.MODE_ECB)
        token = cipher.decrypt(token)
        token = unpad(token, block_size)
        token = str(token, "utf-8")
    except Exception as e:
        return False
    token = str.split(token, "@")
    if len(token) != 4:
        return False
    try:
        w = int(token[0])
        h = int(token[1])
        ua = token[2]
        ts = datetime.datetime.fromtimestamp(int(token[3][:-3]))
    except Exception as e:
        return False
    if w < 100 or h < 100:
        return False
    if "urllib" in ua or "requests" in ua or "PhantomJS" in ua or "Python" in ua or "Scrapy" in ua or "curl" in ua\
            or "Wget" in ua:
        return False
    now = datetime.datetime.now()
    if ts < now + datetime.timedelta(minutes=3) and ts > now - datetime.timedelta(minutes=3):
        return True
    return False