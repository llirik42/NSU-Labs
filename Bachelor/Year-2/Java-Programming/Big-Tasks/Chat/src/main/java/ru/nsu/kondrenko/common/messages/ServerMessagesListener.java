package ru.nsu.kondrenko.common.messages;

import ru.nsu.kondrenko.common.modelobjects.ChatMessage;
import ru.nsu.kondrenko.common.modelobjects.ChatUser;

import java.util.List;

public interface ServerMessagesListener {
    void onLogin(ChatUser chatUser);

    void onLogout(ChatUser chatUser);

    void onErrorResponse(String message);

    void onChatMessage(ChatMessage chatMessage);

    void onUsersListUpdate(List<ChatUser> chatUsersList);
}
