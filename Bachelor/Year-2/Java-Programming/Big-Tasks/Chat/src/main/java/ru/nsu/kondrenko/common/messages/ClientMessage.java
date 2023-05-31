package ru.nsu.kondrenko.common.messages;

import ru.nsu.kondrenko.common.Connection;
import ru.nsu.kondrenko.common.Message;
import ru.nsu.kondrenko.common.ServerContext;

public interface ClientMessage extends Message {
    void handle(ServerContext serverContext, Connection connection);
}
