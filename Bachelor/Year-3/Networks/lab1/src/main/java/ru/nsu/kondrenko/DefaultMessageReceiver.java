package ru.nsu.kondrenko;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.net.DatagramPacket;
import java.net.MulticastSocket;
import java.net.SocketException;

public class DefaultMessageReceiver implements MessageReceiver {
    private final static int kByte = 1024;
    private final static int INFINITE_TIMEOUT = 0;

    private final MulticastSocket multicastSocket;

    public DefaultMessageReceiver(MulticastSocket multicastSocket) {
        this.multicastSocket = multicastSocket;
    }

    @Override
    public Message receiveMessage() throws IOException, ClassNotFoundException {
        final byte[] buffer = new byte[kByte];

        final DatagramPacket datagramPacket = new DatagramPacket(buffer, buffer.length);
        multicastSocket.receive(datagramPacket);

        final ObjectInputStream objectInputStream = new ObjectInputStream(new ByteArrayInputStream(datagramPacket.getData()));
        final Message message = (Message) objectInputStream.readObject();
        message.setAddress(datagramPacket.getSocketAddress());
        objectInputStream.close();

        return message;
    }

    @Override
    public void enableTimeout(int timeout) throws SocketException {
        multicastSocket.setSoTimeout(timeout);
    }

    @Override
    public void disableTimeout() throws SocketException {
        multicastSocket.setSoTimeout(INFINITE_TIMEOUT);
    }
}
