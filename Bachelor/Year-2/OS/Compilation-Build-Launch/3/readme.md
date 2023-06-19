# Компиляция и линковка

```Bash
gcc dynamic_lib.c -shared -o libdynamic_lib.so
```

```Bash
gcc hello3.c -ldynamic_lib -L. -o hello3.out
```

# Запуск

```Bash
LD_LIBRARY_PATH=./ ./hello3.out
```
