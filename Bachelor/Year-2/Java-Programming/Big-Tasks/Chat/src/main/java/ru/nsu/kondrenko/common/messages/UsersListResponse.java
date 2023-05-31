package ru.nsu.kondrenko.common.messages;

import ru.nsu.kondrenko.common.ClientContext;
import ru.nsu.kondrenko.common.modelobjects.ChatUser;

import java.util.List;

public record UsersListResponse(List<ChatUser> chatUsersList) implements Response {
    private static String getXMLForChatUser(ChatUser chatUser) {
        return String.format(
                """
                                <user>
                                    <name> %s </name>
                                    <type> %s </type>
                                </user>
                        """,
                chatUser.name(),
                chatUser.type()
        );
    }

    @Override
    public void handle(ClientContext clientContext, ServerMessagesListener serverMessagesListener) {
        clientContext.updateChatUsersList(chatUsersList);
        serverMessagesListener.onUsersListUpdate(chatUsersList);
    }

    @Override
    public String toXML() {
        final StringBuilder ret = new StringBuilder(
                """
                        <success>
                            <listusers>
                        """
        );

        for (final var it : chatUsersList) {
            ret.append(getXMLForChatUser(it));
        }

        ret.append(
                """
                           </listusers>
                        </success>
                        """
        );

        return ret.toString();
    }
}
