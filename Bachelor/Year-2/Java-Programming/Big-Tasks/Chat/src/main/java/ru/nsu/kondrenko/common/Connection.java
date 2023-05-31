package ru.nsu.kondrenko.common;

import ru.nsu.kondrenko.common.modelobjects.Session;

public interface Connection {
    Session getSession();

    void setSession(Session session);

    void send(Message message);

    boolean isClosed();

    void close();
}
