# Задание

1. Написать [программу](https://github.com/llirik42/NSU-Education/tree/main/OS/CompilationBuildLaunch/src/1) которая выводит фразу “Hello, world!”:
	1. получить исполняемый файл;
	2. посмотреть unresolved symbols (`puts`, `printf`) с помощью `nm`;
	3. посмотреть зависимости (`ldd`);
	4. запустить.
2. Написать [статическую библиотеку](https://github.com/llirik42/NSU-Education/tree/main/OS/CompilationBuildLaunch/src/2) с функцией `print_hello_from_static()` и использовать ее:
	1. посмотреть исполняемый файл на предмет того будет ли функция `print_hello_from_static()` unresolved. Почему?
	2. где находится код этой функции?
3. Написать [динамическую библиотеку](https://github.com/llirik42/NSU-Education/tree/main/OS/CompilationBuildLaunch/src/3) с функцией `print_hello_from_dynamic_lib()` и использовать ее:
	1. посмотреть состояние функции `print_hello_from_dynamic_lib()` в получившимся исполняемом файле. Объяснить увиденное.
4. Написать [динамическую библиотеку](https://github.com/llirik42/NSU-Education/tree/main/OS/CompilationBuildLaunch/src/4) с функцией `print_hello_from_dynamic_runtime_lib()` и загрузить ее с помощью `dlopen(3)`. Объяснить что происходит.