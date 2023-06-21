# Последовательность действий

1. Изменение эффективного *user-id* для всех создаваемых процессов в текущем *bash*-терминале
	```Bash
	sudo su  # Все дальнейшие действия в этом терминале — "Под видом супер-пользователя"
	```

2. Создание файла с "секретной информацией"
	```Bash
	echo "secret information" > file.txt  # Под видом супер-пользователя
	```

3. Изменение прав созданного файла на "чтение только владельцу"
	```Bash
	chmod go-r file.txt  # Под видом супер-пользователя
	```

4. Компиляция исполняемого файла
	```Bash
	gcc reader.c -o reader.out  # Под видом супер-пользователя
	```

5. Установка прав исполняемого файла на "исполнение всем"
	```Bash
	chmod +x reader.out  # Под видом супер-пользователя
	```

6. Попытка (неуспешная) запустить программу, читающую содержимое файла 
	```Bash
	./reader.out file.txt  # Не под видом супер-пользователя
	```
	### Пример вывода
	```
	Real user-id = 1000
	Effective user-id = 1000
	fopen() failed: Permission denied
	```

7. Установка *suid*-бит ***на исполняемый файл***
	```Bash
	chmod +s reader.out  # Под видом супер-пользователя
	```

8. Попытка (успешная) запустить программу, читающую содержимое файла
	```Bash
	./reader.out file.txt  # Не под видом супер-пользователя
	```
	### Пример вывода
	```
	Real user-id = 1000
	Effective user-id = 0
	secret information
	```