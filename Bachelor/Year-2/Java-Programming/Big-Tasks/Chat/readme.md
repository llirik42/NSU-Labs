# Сетевое программирование, сериализация, XML

## Задание

Напишите программу для общения через интернет. Программа должна состоять из двух частей: сервер и клиент. Сервер стартует в качестве отдельного приложения на определенном порту (задано в конфигурации). Клиент в виде приложения на **Swing** подсоединяется к серверу по имени сервера и номеру порта.

## Запуск

### Запуск сервера

Для запуска сервера необходимо, чтобы в папке с ***Chat-1.0.jar*** находилась папка **server** с конфигурационным файлом **config.txt**

#### Пример **config.txt**

```
# PORT=0 - random available port
PORT=8888

# maximum length of the queue of incoming connections
BACKLOG=5

# address of server
BIND_ADDRESS=127.0.0.1

# number of recent messages that will be sent to each new client
RECENT_MESSAGES_COUNT=5

# protocol (OBJECT/XML - ignoring the case)
PROTOCOL=XML

# whether to log all server events or not
LOGGING=true
```

#### Запуск

```Bash
java -cp Chat-1.0.jar ru.nsu.kondrenko.server.Main
```

### Запуск клиентов

```Bash
java -cp Chat-1.0.jar ru.nsu.kondrenko.server.Main  # Client 1
```

```Bash
java -cp Chat-1.0.jar ru.nsu.kondrenko.server.Main  # Client 2
```

...

### Минимальные возможности чата

- каждый участник чата имеет собственный ник, который указывается при присоединению к серверу
- можно посмотреть список участников чата
- можно послать сообщение в чат (всем участникам)

## Требования

1. Клиент показывыает все сообщения, которые отправили в чат с момента подключения + некоторое число, отправленных до; список сообщений обновляется в онлайне.
2. Клиент отображает такие события как: подключение нового человека в чат и уход человека из чата. Сервер должен корректно понимать ситуацию отключения клиента от чата (по таймауту).
3. Сервер должен логгировать все события, которые происходят на его стороне (включается/отключается в конфигурационном файле).
4. Чат работает через **TCP/IP** протокол.
5. Необходимо создать 2 версии клиента/сервера. Первый вариант использует сериализацию/десериализацию Java-объектов для посылки/приема сообщений, второй — использует **XML** сообщения.
6. Клиент и сервер должны поддерживать стандартный протокол для **XML** варианта. Это необходимо для возможности общение между клиентами, созданными разными учениками. Протокол описан ниже. Расширения протокола приветствуются, например можно добавить, чтобы пользователь мог выбрать цвет сообщений.
7. Вначале **XML** сообщения идут 4 байта (`int`) с его длиной. То есть сначала читаются первые 4 байта и узнается длина оставшегося сообщения (в байтах). Затем считывается само сообщение и далее обрабатывается как **XML** документ.

## Примечания

Рекомендуется использовать следующие техники:

1. Сервер слушает порт с помощью класса `java.net.ServerSocket`
2. Клиент подсоединяется к серверу с помощью класса `java.net.Socket`
3. **XML** сообщение читать с помощью **DOM-parser**: `DocumentBuilderFactory.newInstance().newDocumentBuilder().parse()`
4. Сериализация/десериализация объекта выполняется через классы `ObjectInputStream` и `ObjectOutputStream`

## Минимальный протокол взаимодействия для XML сообщений

### Регистрация

#### Client message

```XML

<command name="login">
    <name> USER_NAME </name>
    <type> CHAT_CLIENT_NAME </type>
</command>
```

#### Server error answer

```XML
<error>
    <message> REASON </message>
</error>
```

#### Server success answer

```XML

<success>
    <session> UNIQUE_SESSION_ID </session>
</success>
```

### Запрос списка пользователей чата

#### Client message

```XML
<command name="list">
    <session> UNIQUE_SESSION_ID </session>
</command>
```

#### Server error answer

```XML

<error>
    <message> REASON </message>
</error>
```

#### Server success answer

```XML
<success>
    <listusers>
        <user>
            <name> USER_1 </name>
            <type> CHAT_CLIENT_1 </type>
        </user>
        …
        <user>
            <name> USER_N </name>
            <type> CHAT_CLIENT_N </type>
        </user>
    </listusers>
</success>
```

### Сообщение от клиента серверу

#### Client message

```XML
<command name="message">
    <message> MESSAGE </message>
    <session> UNIQUE_SESSION_ID </session>
</command>
```

#### Server error answer

```XML
<error>
    <message> REASON </message>
</error>
```

#### Server success answer

```XML

<success>
</success>
```

### Сообщение от сервера клиенту

#### Server message

```XML

<event name="message">
    <message> MESSAGE </message>
    <name> CHAT_NAME_FROM </name>
</event>
```

### Отключение

#### Client message

```XML

<command name="logout">
    <session> UNIQUE_SESSION_ID </session>
</command>
```

#### Server error answer

```XML
<error>
    <message> REASON </message>
</error>
```

#### Server success answer

```XML
<success>
</success>
```

### Новый клиент

#### Server message

```XML
<event name="userlogin">
    <name> USER_NAME </name>
</event>
```

### Клиент отключился

#### Server message

```XML
<event name="userlogout">
    <name> USER_NAME </name>
</event>
```
