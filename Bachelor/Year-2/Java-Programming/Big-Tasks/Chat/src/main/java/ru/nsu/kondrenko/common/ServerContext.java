package ru.nsu.kondrenko.common;

import ru.nsu.kondrenko.common.messages.Event;
import ru.nsu.kondrenko.common.modelobjects.ChatUser;
import ru.nsu.kondrenko.common.modelobjects.Session;

import java.util.List;

public interface ServerContext {
    void removeConnectionBySession(Session session);

    void addChatUser(ChatUser chatUser, Session session);

    ChatUser getChatUserBySession(Session session);

    void removeChatUserBySession(Session session);

    List<ChatUser> getChatUsers();

    boolean containsChatUser(ChatUser chatUser);

    void addChatEvent(Event event);

    List<Event> getRecentChatEvents();

    Session createSession();

    List<Connection> getOpenConnections();
}
