package ru.nsu.kondrenko.common.messages;

import ru.nsu.kondrenko.common.Connection;
import ru.nsu.kondrenko.common.ServerContext;
import ru.nsu.kondrenko.common.modelobjects.ChatMessage;
import ru.nsu.kondrenko.common.modelobjects.Session;

public record MessageSendingRequest(Session session, String messageContent) implements NonLoginRequest {
    private static final String BLANK_MESSAGE_MESSAGE = "Message can't be blank";

    @Override
    public void handle(ServerContext serverContext, Connection connection) {
        if (messageContent.isBlank()) {
            connection.send(new ErrorResponse(BLANK_MESSAGE_MESSAGE));
            return;
        }

        connection.send(new SuccessResponse());

        final ChatMessage newChatMessage = new ChatMessage(
                messageContent,
                serverContext.getChatUserBySession(session)
        );

        final Event chatMessageEvent = new ChatMessageEvent(newChatMessage);
        serverContext.addChatEvent(chatMessageEvent);
        final Iterable<Connection> openConnections = serverContext.getOpenConnections();
        for (final var it : openConnections) {
            it.send(chatMessageEvent);
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
                        <command name="message">
                            <message> %s </message>
                            <session> %s </session>
                        </command>
                        """,
                messageContent,
                session
        );
    }
}
