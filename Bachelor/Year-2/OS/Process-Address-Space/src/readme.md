# Компиляция и линковка

```Bash
gcc chunk.c my_heap.c -shared -o libheap.so -fPIC
```

```Bash
gcc main.c -lheap -L. -o main
```

# Запуск

```Bash
LD_LIBRARY_PATH=./ ./main
```
