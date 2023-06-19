# Компиляция и линковка

```Bash
gcc static_lib.c -c -o static_lib.o
```

```Bash
ar rc libstatic_lib.a static_lib.o
```

```Bash
gcc hello2.c -lstatic_lib -L. -o hello2.out
```

# Запуск

```Bash
./hello2.out
```
