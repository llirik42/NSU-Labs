# Компиляция и линковка

```Bash
gcc client.c utils.c -o client.out
```

```Bash
gcc server.c utils.c clients_container.c -o server.out
```

# Запуск

## Запуск сервера

```Bash
./server.out
```

## Запуск клиентов

```Bash
./client.out  # Client 1
```

```Bash
./client.out  # Client 2
```

$\dots$
