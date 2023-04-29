# Компиляция и линковка

```Bash
gcc hello_without_syscall.s -c -o hello_without_syscall.o
```

```Bash
ld hello_without_syscall.o -o hello_without_syscall
```

# Запуск

```Bash
./hello_without_syscall
```
