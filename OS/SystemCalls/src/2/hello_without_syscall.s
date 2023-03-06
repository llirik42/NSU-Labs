.data
msg:
    .ascii "Hello, world! (without syscall)\n"

.text
    .global _start

_start:
    movq $1, %rax
    movq $1, %rdi
    movq $msg, %rsi
    movq $32, %rdx
    syscall

    movq $60, %rax
    movq $0, %rdi
    syscall
