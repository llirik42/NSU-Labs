package ru.nsu.kondrenko.common.messages;

import ru.nsu.kondrenko.common.ClientContext;
import ru.nsu.kondrenko.common.modelobjects.ChatUser;

public record LogoutEvent(ChatUser chatUser) implements Event {
    @Override
    public void handle(ClientContext clientContext, ServerMessagesListener serverMessagesListener) {
        clientContext.removeChatUser(chatUser);
        serverMessagesListener.onLogout(chatUser);
    }

    @Override
    public String toXML() {
        return String.format(
                """
                        <event name="userlogout">
                            <name> %s </name>
                        </event>
                        """,
                chatUser.name()
        );
    }
}
