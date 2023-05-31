package ru.nsu.kondrenko.common.messages;

import ru.nsu.kondrenko.common.Connection;
import ru.nsu.kondrenko.common.ServerContext;
import ru.nsu.kondrenko.common.modelobjects.ChatUser;
import ru.nsu.kondrenko.common.modelobjects.Session;

public record LoginRequest(ChatUser chatUser) implements Request {
    private static final String BLANK_NAME_MESSAGE = "User name can't be blank";

    @Override
    public void handle(ServerContext serverContext, Connection connection) {
        if (chatUser.name().isBlank()) {
            connection.send(new ErrorResponse(BLANK_NAME_MESSAGE));
            return;
        }

        if (serverContext.containsChatUser(chatUser)) {
            connection.send(new ErrorResponse("User with your name is already in the chat"));
            return;
        }

        final Session newSession = serverContext.createSession();
        connection.setSession(newSession);
        serverContext.addChatUser(chatUser, newSession);
        connection.send(new LoginResponse(newSession));

        final Iterable<Event> recentChatEvents = serverContext.getRecentChatEvents();
        for (final var it : recentChatEvents) {
            connection.send(it);
        }

        final Event loginEvent = new LoginEvent(chatUser);
        serverContext.addChatEvent(loginEvent);
        final Iterable<Connection> openConnections = serverContext.getOpenConnections();
        for (final var it : openConnections) {
            it.send(loginEvent);
        }
    }

    @Override
    public String toXML() {
        return String.format(
                """
                        <command name="login">
                            <name> %s </name>
                            <type> %s </type>
                        </command>
                        """,
                chatUser.name(),
                chatUser.type()
        );
    }
}
