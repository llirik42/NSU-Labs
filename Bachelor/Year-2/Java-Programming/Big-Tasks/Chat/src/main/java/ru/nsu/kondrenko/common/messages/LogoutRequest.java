package ru.nsu.kondrenko.common.messages;

import ru.nsu.kondrenko.common.Connection;
import ru.nsu.kondrenko.common.ServerContext;
import ru.nsu.kondrenko.common.modelobjects.ChatUser;
import ru.nsu.kondrenko.common.modelobjects.Session;

import java.util.List;

public record LogoutRequest(Session session) implements NonLoginRequest {
    @Override
    public void handle(ServerContext serverContext, Connection connection) {
        if (!connection.isClosed()) {
            connection.close();
            connection.send(new SuccessResponse());
        }

        final Session sessionOfConnection = connection.getSession();
        final ChatUser chatUser = serverContext.getChatUserBySession(sessionOfConnection);
        serverContext.removeConnectionBySession(sessionOfConnection);
        serverContext.removeChatUserBySession(sessionOfConnection);

        final Event logoutEvent = new LogoutEvent(chatUser);
        serverContext.addChatEvent(logoutEvent);
        final List<Connection> openConnections = serverContext.getOpenConnections();
        for (final var it : openConnections) {
            it.send(logoutEvent);
        }
    }

    @Override
    public Session getSession() {
        return session;
    }

    @Override
    public String toXML() {
        return String.format(
                """
                        <command name="logout">
                            <session> %s </session>
                        </command>
                        """,
                session
        );
    }
}
