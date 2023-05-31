package ru.nsu.kondrenko.common.messages;

import ru.nsu.kondrenko.common.ClientContext;
import ru.nsu.kondrenko.common.Message;

public interface ServerMessage extends Message {
    void handle(ClientContext clientContext, ServerMessagesListener serverMessagesListener);
}
