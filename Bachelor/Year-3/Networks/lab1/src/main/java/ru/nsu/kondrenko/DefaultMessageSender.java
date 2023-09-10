package ru.nsu.kondrenko;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;

public class DefaultMessageSender implements MessageSender {
    private final static int kByte = 1024;

    private final MulticastSocket multicastSocket;
    private final InetAddress inetAddress;
    private final int port;

    public DefaultMessageSender(MulticastSocket multicastSocket, InetAddress inetAddress, int port) {
        this.multicastSocket = multicastSocket;
        this.inetAddress = inetAddress;
        this.port = port;
    }

    @Override
    public void sendMessage(final Message message) throws IOException {
        final ByteArrayOutputStream data = new ByteArrayOutputStream(kByte);

        final ObjectOutputStream objectOutputStream = new ObjectOutputStream(data);
        objectOutputStream.writeObject(message);

        final DatagramPacket datagramPacket = new DatagramPacket(
                data.toByteArray(),
                data.size(),
                inetAddress,
                port);

        multicastSocket.send(datagramPacket);
    }
}
