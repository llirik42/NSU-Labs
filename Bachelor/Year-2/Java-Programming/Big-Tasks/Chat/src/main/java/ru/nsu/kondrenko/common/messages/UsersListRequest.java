package ru.nsu.kondrenko.common.messages;

import ru.nsu.kondrenko.common.Connection;
import ru.nsu.kondrenko.common.ServerContext;
import ru.nsu.kondrenko.common.modelobjects.ChatUser;
import ru.nsu.kondrenko.common.modelobjects.Session;

import java.util.List;

public record UsersListRequest(Session session) implements NonLoginRequest {
    @Override
    public void handle(ServerContext serverContext, Connection connection) {
        final List<ChatUser> chatUsers = serverContext.getChatUsers();
        connection.send(new UsersListResponse(chatUsers));
    }

    @Override
    public Session getSession() {
        return session;
    }

    @Override
    public String toXML() {
        return String.format(
                """
                        <command name="list">
                            <session> %s </session>
                        </command>
                        """,
                session
        );
    }
}
