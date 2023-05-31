package ru.nsu.kondrenko.client.model;

import ru.nsu.kondrenko.common.messages.ServerMessagesListener;

public interface ClientListener extends ServerMessagesListener {
    void onClientDisconnect();

    void onException(Exception exception);
}
