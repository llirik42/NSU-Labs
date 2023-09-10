package ru.nsu.kondrenko;

import ru.nsu.kondrenko.messages.ByMessage;
import ru.nsu.kondrenko.messages.HiMessage;

import java.net.InetAddress;
import java.net.MulticastSocket;
import java.util.LinkedList;
import java.util.List;

public class Main {
    public static final int ERROR = 1;

    public static final int PORT = 9999;

    public static void main(String[] args) {
        if (args.length != 1) {
            System.err.println("Incorrect number of args. The only argument must be address of multicast-group.");
            System.exit(ERROR);
        }

        final String multicastGroupAddressString = args[0];

        try {
            final InetAddress multicastGroupAddress = InetAddress.getByName(multicastGroupAddressString);

            if (!multicastGroupAddress.isMulticastAddress()) {
                System.err.println("The address must be multicast.");
                System.exit(ERROR);
            }

            final MulticastSocket multicastSocket = new MulticastSocket(PORT);
            multicastSocket.joinGroup(multicastGroupAddress);

            final MessageSender sender = new DefaultMessageSender(multicastSocket, multicastGroupAddress, PORT);
            final MessageReceiver receiver = new DefaultMessageReceiver(multicastSocket);

            Runtime.getRuntime().addShutdownHook(new Thread(() -> {
                final Message byMessage = new ByMessage();

                try {
                    sender.sendMessage(byMessage);
                    multicastSocket.leaveGroup(multicastGroupAddress);
                } catch (Exception ignored) {

                }
            }));

            final Message hiMessage = new HiMessage();
            sender.sendMessage(hiMessage);

            final List<AppCopy> liveAppCopies = new LinkedList<>();
            while (true) {
                final Message message = receiver.receiveMessage();
                message.handle(liveAppCopies, sender, receiver);
            }
        } catch (Exception exception) {
            System.err.println(exception.getLocalizedMessage());
            System.exit(ERROR);
        }
    }
}
