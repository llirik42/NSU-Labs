package ru.nsu.kondrenko.server.serverdata;

import ru.nsu.kondrenko.common.Connection;
import ru.nsu.kondrenko.common.messages.Event;
import ru.nsu.kondrenko.common.modelobjects.ChatUser;
import ru.nsu.kondrenko.common.modelobjects.Session;

import java.util.List;

public interface ServerData {
    void addConnection(Connection connection);

    List<Connection> getConnectionsList();

    void removeConnectionBySession(Session session);

    void addChatUser(ChatUser chatUser, Session session);

    void removeChatUserBySession(Session session);

    ChatUser getChatUserBySession(Session session);

    List<ChatUser> getChatUsers();

    void addChatEvent(Event event);

    List<Event> getRecentChatEvents(int count);

    boolean containsUser(ChatUser chatUser);

    boolean containsUserBySession(Session session);
}
