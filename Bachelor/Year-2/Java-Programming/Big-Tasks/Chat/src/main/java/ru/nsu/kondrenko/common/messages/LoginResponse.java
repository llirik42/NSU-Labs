package ru.nsu.kondrenko.common.messages;

import ru.nsu.kondrenko.common.ClientContext;
import ru.nsu.kondrenko.common.modelobjects.Session;

public record LoginResponse(Session session) implements Response {
    @Override
    public void handle(ClientContext clientContext, ServerMessagesListener serverMessagesListener) {
        clientContext.setSession(session);
        clientContext.getConnection().send(new UsersListRequest(session));
    }

    @Override
    public String toXML() {
        return String.format(
                """
                        <success>
                            <session> %s </session>
                        </success>
                        """,
                session
        );
    }
}
