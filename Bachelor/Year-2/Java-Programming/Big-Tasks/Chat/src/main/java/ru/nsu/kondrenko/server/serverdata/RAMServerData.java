package ru.nsu.kondrenko.server.serverdata;

import ru.nsu.kondrenko.common.Connection;
import ru.nsu.kondrenko.common.messages.Event;
import ru.nsu.kondrenko.common.modelobjects.ChatUser;
import ru.nsu.kondrenko.common.modelobjects.Session;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class RAMServerData implements ServerData {
    private final List<Connection> connectionsList = new ArrayList<>();
    private final List<Event> chatEventsList = new ArrayList<>();
    private final Map<Session, ChatUser> chatUsersMap = new HashMap<>();

    @Override
    public void addConnection(Connection connection) {
        connectionsList.add(connection);
    }

    @Override
    public List<Connection> getConnectionsList() {
        return connectionsList;
    }

    @Override
    public void removeConnectionBySession(Session session) {
        connectionsList.removeIf(it -> it.getSession().equals(session));
    }

    @Override
    public void addChatUser(ChatUser chatUser, Session session) {
        chatUsersMap.put(session, chatUser);
    }

    @Override
    public void removeChatUserBySession(Session session) {
        chatUsersMap.remove(session);
    }

    @Override
    public ChatUser getChatUserBySession(Session session) {
        return chatUsersMap.get(session);
    }

    @Override
    public List<ChatUser> getChatUsers() {
        return new ArrayList<>(chatUsersMap.values());
    }

    @Override
    public void addChatEvent(Event event) {
        chatEventsList.add(event);
    }

    @Override
    public List<Event> getRecentChatEvents(int count) {
        return chatEventsList.size() < count
                ? chatEventsList
                : chatEventsList.subList(chatEventsList.size() - count, chatEventsList.size());
    }

    @Override
    public boolean containsUser(ChatUser chatUser) {
        for (final var it : chatUsersMap.values()) {
            if (it.equals(chatUser)) {
                return true;
            }
        }

        return false;
    }

    @Override
    public boolean containsUserBySession(Session session) {
        return chatUsersMap.containsKey(session);
    }
}
