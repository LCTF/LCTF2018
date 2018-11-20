#!/usr/bin/env python2

import data

#ciphertext = 

init_iv = [ord(s) for s in "701cc9a49e30d35bebb81fbafc3c9bea"]
init_key = [ord(s) for s in ("KomeijiSatori"*4)]

def encrypto(data):
    r = []
    for i in range(32):
        c = 0x39 ^ 0xFF ^ init_key[i] ^ data[i]
        r.append(c)
    return r

def xordata(one, two):
    r = []
    for i in range(32):
        c = one[i] ^ two[i]
        r.append(c)
    return r


def decode(data):
    iv = init_iv

    result = []
    for d in data:
        iv = encrypto(iv)
        plaintext = xordata(d, iv)
        result += plaintext

    return result

def split_by(data, n):
    r = []
    
    for k in xrange(0, len(data), n):
        r.append([])
        for i in range(k, k+n):
            r[-1].append(data[i])

    return r

def main():
    d = split_by(data.ciphertext, 32)
    #d = split_by(ciphertext, 32)

    r = decode(d)
    msg = "".join([chr(s) for s in r if s != 5])
    print msg




























if __name__ == '__main__':
    main()

