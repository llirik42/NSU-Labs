package ru.nsu.kondrenko.common.messages;

import ru.nsu.kondrenko.common.ClientContext;
import ru.nsu.kondrenko.common.modelobjects.ChatUser;

public record LoginEvent(ChatUser chatUser) implements Event {
    @Override
    public void handle(ClientContext clientContext, ServerMessagesListener serverMessagesListener) {
        clientContext.addChatUser(chatUser);
        serverMessagesListener.onLogin(chatUser);
    }

    @Override
    public String toXML() {
        return String.format(
                """
                        <event name="userlogin">
                            <name> %s </name>
                        </event>
                        """,
                chatUser.name()
        );
    }
}
