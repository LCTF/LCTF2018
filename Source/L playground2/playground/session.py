import base64
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

def session_decode(info):
    info_list = str.split(info, ".")
    if len(info_list) != 2:
        raise Exception("error info")
    info_ = decode(info_list[0])
    if not hash_verify(info_list[1], info_):
        raise Exception("hash wrong")
    return info_

