package ru.nsu.kondrenko.messages;

import ru.nsu.kondrenko.AppCopy;
import ru.nsu.kondrenko.Message;
import ru.nsu.kondrenko.MessageReceiver;
import ru.nsu.kondrenko.MessageSender;

import java.util.List;

public class HiResponse extends Message {
    @Override
    public void handle(List<AppCopy> appCopies, MessageSender sender, MessageReceiver receiver) {
        appCopies.add(new AppCopy(getPid(), getAddress()));
    }
}
