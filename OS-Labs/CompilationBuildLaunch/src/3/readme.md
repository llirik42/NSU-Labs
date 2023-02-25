# Компиляция и линковка
    $ gcc dynamic_lib.c -shared -o libdynamic_lib.so
    
    $ gcc hello3.c -ldynamic_lib -L. -o hello3
# Запуск
    $ LD_LIBRARY_PATH=./ ./hello3

**либо**

    $ export LD_LIBRARY_PATH=./
    $ ./hello3 
