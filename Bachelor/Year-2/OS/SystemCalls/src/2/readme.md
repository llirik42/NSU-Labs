# Компиляция и линковка
    $ gcc hello_without_syscall.s -c -o hello_without_syscall.o

    $ ld hello_without_syscall.o -o hello_without_syscall
# Запуск
    $ ./hello_without_syscall