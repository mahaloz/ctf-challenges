// gcc -fPIC -nostdlib -no-pie -s signal_silence.c -o signal_silence

volatile void one_true_gadget(void)
{
    asm("mov $0xf, %eax;" // sigreturn
        "syscall;");
}
asm("poprax:"
    "pop %rax;"
    "ret;");

char* useful_string = "/bin/sh";
char useful_byte = 'i';

asm("nop;"
    "nop;"
    "nop;"
    "nop;"
    "nop;"
    "nop;"
    "nop;"
    "nop;"
    "nop;"
    "nop;"
    "nop;"
    "nop;"
   );

int main()
{
    char buf[183];

    asm("mov %0, %%rsi;"
        :
        :"r"(buf)
        :"%rsi"
       );
    asm("xor %rax, %rax;"
        "xor %rdi, %rdi;"
        "mov $0x200, %rdx;"
        "syscall;");
    return 0;
}

void _start()
{
    main();
    asm("xor %rdi, %rax;"
        "mov $60, %rax;"
        "syscall;"
       );

}

