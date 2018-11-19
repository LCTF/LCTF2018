#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-

__metaclass__ = type

import random
import struct

def _bytelist2long(list):
    imax = len(list)//4
    hl = [0] * imax

    j = 0
    i = 0
    while i < imax:
        b0 = (ord(list[j]))
        b1 = ((ord(list[j+1]))) << 8
        b2 = ((ord(list[j+2]))) << 16
        b3 = ((ord(list[j+3]))) << 24
        hl[i] = b0 | b1 |b2 | b3
        i = i+1
        j = j+4
    return hl

def _rotateLeft(x, n):
    return (x << n) | (x >> (32-n))

def F(x, y, z):
    return (x & y) | ((~x) & z)

def G(x, y, z):
    return (x & z) | (y & (~z))

def H(x, y, z):
    return x ^ y ^ z

def I(x, y, z):
    return y ^ (x | (~z))

def XX(func, a, b, c, d, x, s, ac):
    res = 0
    res = res + a + func(b, c, d)
    res = res + x 
    res = res + ac
    res = res & 0xffff
    res = _rotateLeft(res, s)
    res = res & 0xffff
    res = res + b

    return res & 0xffff

# Message-Digest Algorithm
class MDA:
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

    def _transform(self, inp):
        a, b, c, d = A, B, C, D = self.A, self.B, self.C, self.D
        S11, S12, S13, S14 = 7, 12, 17, 22
        a = XX(F, a, b, c, d, inp[ 0], S11, 0xA478) 
        d = XX(F, d, a, b, c, inp[ 1], S12, 0xB756) 
        c = XX(F, c, d, a, b, inp[ 2], S13, 0x70DB) 
        b = XX(F, b, c, d, a, inp[ 3], S14, 0xCEEE) 

        S21, S22, S23, S24 = 5, 9, 14, 20
        a = XX(G, a, b, c, d, inp[ 1], S21, 0x2562)
        b = XX(G, b, c, d, a, inp[ 0], S24, 0xC7AA)
        c = XX(G, c, d, a, b, inp[ 3], S23, 0x0D87)
        d = XX(G, d, a, b, c, inp[ 2], S22, 0xA3F8)

        S31, S32, S33, S34 = 4, 11, 16, 23
        a = XX(H, a, b, c, d, inp[ 1], S31, 0xEA44)
        d = XX(H, d, a, b, c, inp[ 0], S32, 0x27FA)
        c = XX(H, c, d, a, b, inp[ 3], S33, 0x3085)
        b = XX(H, b, c, d, a, inp[ 2], S34, 0x5665)

        S41, S42, S43, S44 = 6, 10, 15, 21
        a = XX(I, a, b, c, d, inp[ 0], S41, 0x2244) 
        d = XX(I, d, a, b, c, inp[ 3], S42, 0xCC92)
        b = XX(I, b, c, d, a, inp[ 1], S44, 0x5DD1)
        c = XX(I, c, d, a, b, inp[ 2], S43, 0xD2BB)

        A = (A + a) & 0x7fff
        B = (B + b) & 0x7fff
        C = (C + c) & 0x7fff
        D = (D + d) & 0x7fff

        self.A, self.B, self.C, self.D = A, B, C, D

    def update(self, inBuf):
        leninBuf = len(inBuf)
        index = (self.count[0] >> 3) & 0xf

        self.count[0] = self.count[0] + (leninBuf << 3)
        if self.count[0] < (leninBuf << 3):
            self.count[1] = self.count[1] + 1
        self.count[1] = self.count[1] + (leninBuf >> 29)

        partLen = 16 - index

        if leninBuf >= partLen:
            self.input[index:] = list(inBuf[:partLen])
            self._transform(_bytelist2long(self.input))
            i = partLen
            while i + 15 < leninBuf:
                self._transform(_bytelist2long(list(inBuf[i:i+16])))
                i = i + 16
            else:
                self.input = list(inBuf[i:leninBuf])
        else:
            i = 0
            self.input = self.input + list(inBuf)

    def insert(self, inBuf):
        self.init()
        self.update(inBuf)

    def digest(self):
        A = self.A
        B = self.B
        C = self.C
        D = self.D
        input = [] + self.input
        count = [] + self.count

        index = (self.count[0] >> 3) & 0xf

        if index < 8:
            padLen = 8 - index
        else:
            padLen = 24 - index

        padding = ['\200'] + ['\000'] * 15

        self.update(padding[:padLen])

        bits = _bytelist2long(self.input[:8]) + count
        self._transform(bits)

        digest = struct.pack("<hhhh", self.A, self.B, self.C, self.D)

        self.A = A 
        self.B = B
        self.C = C
        self.D = D
        self.input = input 
        self.count = count
        return digest

    def hexdigest(self):
        return ''.join(['%02x' % ord(chr(c)) for c in self.digest()])

    def grouping(self, inBufGroup):
        hexdigest_group = ""
        for inBuf in inBufGroup:
            self.insert(inBuf)
            hexdigest_group += self.hexdigest()
        return hexdigest_group


