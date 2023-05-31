package ru.nsu.kondrenko.client.model;

import ru.nsu.kondrenko.common.modelobjects.ChatUser;
import ru.nsu.kondrenko.common.protocols.Protocol;

public interface Client {
    void join(int port, String serverAddress, ChatUser chatUser, Protocol protocol);

    void leave();

    void shutdown();

    void sendChatMessage();

    void insertLineBreak();
}
