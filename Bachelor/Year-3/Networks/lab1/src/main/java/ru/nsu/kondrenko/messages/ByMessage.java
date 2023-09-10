package ru.nsu.kondrenko.messages;

import ru.nsu.kondrenko.*;

import java.util.List;

public class ByMessage extends Message {
    @Override
    public void handle(List<AppCopy> appCopies, MessageSender sender, MessageReceiver receiver) {
        for (final var it : appCopies) {
            if (it.pid() == getPid()) {
                appCopies.remove(it);
                break;
            }
        }

        MessageUtils.printAppCopies(appCopies);
    }
}
