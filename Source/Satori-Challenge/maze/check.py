#!/usr/bin/env python2

flag = "LCTF{Y0ur_fl4g_1s_wr0ng}"
cipher = "IQURUEURYEU#WRTYIPUYRTI!WTYTE!WOR%Y$W#RPUEYQQ^EE"
table = "QWERTYUIOP!@#$%^"
org_iv = [ord(s) for s in "PCBC"]
org_key = [ord(s) for s in "LCTF"] 


def xor_map(l, x):
    t = [d ^ x for d in l]
    return t

def plus_map(l, x):
    t = []
    for d in l:
        t.append(d ^ x ^ 0x11)
    return t

def xor_add(one, two):
    t = []
    assert len(one) == len(two), "xor add failed"
    for i in range(len(one)):
        x = one[i]
        y = two[i]
        t.append(x ^ y)
    return t

def add_add(one, two):
    t = []
    assert len(one) == len(two), "add add failed"
    for i in range(len(one)):
        x = one[i]
        y = two[i]
        t.append(x ^ y ^ 0x11)
    return t

def show_list(d):
    s = ["%#x " % s for s in d]
    print "".join(s)


# ------------------------------------------


# decode base16
data = []
for i in range(0, len(cipher), 2):
    c = table.index(cipher[i]) << 4
    c = c | table.index(cipher[i+1])
    data.append(c)

print "base16 len: %d" % len(data)
assert 2 * len(data) == len(cipher), "base16 failed"


# split by 4
dsp4 = []
for i in range(0, len(data), 4):
    dsp4.append([data[i], data[i+1], data[i+2], data[i+3]])

print "dsp4 len: %d" % len(dsp4)
assert 4 * len(dsp4) == len(data), "dsp4 failed"


# set iv
ivs = []
ivs.append([ord('5') ^ s for s in org_iv])
for i in range(len(dsp4) - 1):
    ivs.append(dsp4[i])

print "ivs len: %d" % len(ivs)
assert len(dsp4) == len(ivs), "dsp4 != ivs!!!!"


# get flag
result = []
for i in range(len(dsp4)):
    _key = [s ^ 0x35 ^ 0x34 for s in org_key]
    _iv = ivs[i]
    _data = dsp4[i]

    if(i != len(dsp4) - 1):
        _data = xor_map(_data, 0x35)

    _data = add_add(_data, _key)
    _data = xor_add(_data, _iv)
    _data = plus_map(_data, 0x31)
    _data = plus_map(_data, 1)

    result += _data


# print it
message = "".join([chr(s) for s in result])
assert message == flag, "Check failed!"
print message


