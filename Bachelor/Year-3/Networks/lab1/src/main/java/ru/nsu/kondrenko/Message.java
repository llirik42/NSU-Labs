package ru.nsu.kondrenko;

import java.io.IOException;
import java.io.Serializable;
import java.net.SocketAddress;
import java.util.List;

public abstract class Message implements Serializable {
    private final long pid;
    private SocketAddress address;

    public Message() {
        pid = ProcessHandle.current().pid();
    }

    public abstract void handle(List<AppCopy> appCopies, MessageSender sender, MessageReceiver receiver) throws IOException, ClassNotFoundException;

    public long getPid() {
        return pid;
    }

    public SocketAddress getAddress() {
        return address;
    }

    public void setAddress(SocketAddress address) {
        this.address = address;
    }
}
