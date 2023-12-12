# Компиляция и линковка

```Bash
gcc main.c storage.c utils.c -D<TYPE> -o main.out 
```

где *Type* может быть одним из `SPIN`, `MUTEX`, `MY_SPIN`, `MY_MUTEX`. Если не указать опцию `-D`, то будет использоваться *rwlock*.

# Запуск

```Bash
./main.out -h
```

```Bash
./main.out -s SIZE -n NUMBER -t TIME
```
