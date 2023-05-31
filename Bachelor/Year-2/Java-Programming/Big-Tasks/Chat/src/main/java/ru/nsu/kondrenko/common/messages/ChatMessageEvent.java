package ru.nsu.kondrenko.common.messages;

import ru.nsu.kondrenko.common.ClientContext;
import ru.nsu.kondrenko.common.modelobjects.ChatMessage;

public record ChatMessageEvent(ChatMessage chatMessage) implements Event {
    @Override
    public void handle(ClientContext clientContext, ServerMessagesListener serverMessagesListener) {
        serverMessagesListener.onChatMessage(chatMessage);
    }

    @Override
    public String toXML() {
        return String.format(
                """
                        <event name="message">
                            <message> %s </message>
                            <name> %s </name>
                        </event>
                        """,
                chatMessage.content(),
                chatMessage.sender().name()
        );
    }
}
