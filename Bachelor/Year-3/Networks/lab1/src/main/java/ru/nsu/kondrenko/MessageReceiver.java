package ru.nsu.kondrenko;

import java.io.IOException;
import java.net.SocketException;

public interface MessageReceiver {
    Message receiveMessage() throws IOException, ClassNotFoundException;

    void enableTimeout(int timeout) throws SocketException;

    void disableTimeout() throws SocketException;
}
