Савенко Д.В., <ds@dsavenko.com>

# Основы работы с сетью

[HTTP](https://ru.wikipedia.org/wiki/HTTP) — это текстовый протокол передачи файлов. В простейшем случае, клиент открывает соединение с сервером и отправляет ему запрос вида:

    GET /path/to/resource HTTP/1.0
    <пустая строка>

`/path/to/resource` — это то, что в браузере у вас после хоста. Например, для http://table.nsu.ru/ — это будет просто "/." А для http://table.nsu.ru/group/21203 — это будет "/group/21203".

На запрос сервер отвечает:
    
    HTTP/1.0 200 OK
    <служебная информация (опционально)>
    <пустая строка>
    <содержимое файла (опционально)>

и закрывает соединение. Если же ресурс был не найден, сервер отвечает

    HTTP/1.0 404 Not Found
    <служебная информация (опционально)>
    <пустая строка>
    <сообщение об ошибке (опционально)>

и закрывает соединение.

## Задание

1. [*Тривиальный HTTP-сервер*](src/main/java/ru/nsu/kondrenko/task1/Main/java)

    Создайте тривиальный HTTP-сервер. Выделите папку на вашем диске, где у вас будут текстовые файлы. Если к серверу обращаются с запросом получения /filename.txt, и такой файл есть в этой папке, он его отдает. Если такого файла нет, или путь неправильный, он возвращает 404. В целях безопасности не разрешайте другие пути ресурсов, кроме / + имя файла (иначе у вас можно будет прочитать любой файл на компьютере). Запустите сервер на порте 8080. Откройте в браузере адрес http://127.0.0.1:8080/filename.txt, убедитесь, что браузер показал содержимое файла.
   
   ### Дополнительно

   Добавьте поддержку одновременно нескольких клиентов.


2. [*Тривиальный HTTP-клиент*](src/main/java/ru/nsu/kondrenko/task2/Main.java)
   
   Создайте тривиальный HTTP-клиент. В аргументах командной строки ему передается host и resource. Программа коннектится к этому хосту на порт 80 (стандартный порт для HTTP), скачивает этот ресурс и выводит на консоль его содержимое (альтернативно, сохраняет в файл). Корректно обрабатывайте ситуацию, когда ресурс не найден. Запустите программу с аргументами google.com /, посмотрите, что получилось.

   ### Дополнительно
   
   Добавьте разбор и вывод на консоль HTTP headers.


## Общее требование
Для работы с сетью используйте только классы `Socket` и `ServerSocket`. Не используйте более высокоуровневое API.