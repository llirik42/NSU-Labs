package ru.nsu.kondrenko.messages;

import ru.nsu.kondrenko.*;

import java.io.IOException;
import java.util.List;

public class HiMessage extends Message {
    @Override
    public void handle(List<AppCopy> appCopies, MessageSender sender, MessageReceiver receiver) throws IOException {
        appCopies.clear();
        receiver.enableTimeout(50); // Timeout

        final Message hiResponse = new HiResponse();
        sender.sendMessage(hiResponse);

        try {
            while (true) {
                final Message message = receiver.receiveMessage();
                appCopies.add(new AppCopy(message.getPid(), message.getAddress()));
            }
        } catch (Exception exception) {
            receiver.disableTimeout();
            MessageUtils.printAppCopies(appCopies);
        }
    }
}
