package ru.nsu.kondrenko.common.protocols;

import ru.nsu.kondrenko.common.Message;
import ru.nsu.kondrenko.common.messages.InvalidMessageException;

public interface MessageInputStream extends MessageStream {
    Message readMessage() throws MessageStreamException, InvalidMessageException;
}

