# Компиляция и линковка
```Bash
    gcc static_lib.c -c -o static_lib.o

    ar rc libstatic_lib.a static_lib.o

    gcc hello2.c -lstatic_lib -L. -o hello2
```

# Запуск

```Bash
    ./hello2
```
