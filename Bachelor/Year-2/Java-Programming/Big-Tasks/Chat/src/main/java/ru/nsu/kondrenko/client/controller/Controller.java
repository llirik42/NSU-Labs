package ru.nsu.kondrenko.client.controller;

import ru.nsu.kondrenko.client.model.Client;
import ru.nsu.kondrenko.client.view.SwingView;
import ru.nsu.kondrenko.common.Utils;
import ru.nsu.kondrenko.common.modelobjects.ChatUser;
import ru.nsu.kondrenko.common.protocols.Protocol;

import java.awt.event.*;
import java.util.function.Predicate;
import java.util.function.Supplier;

public class Controller extends WindowAdapter implements ActionListener, KeyListener {
    private Client client;
    private SwingView view;
    private boolean ctrlReleased = true;

    public void setClient(Client client) {
        this.client = client;
    }

    public void setView(SwingView view) {
        this.view = view;
    }

    public void start() {
        final String serverAddress = inputServerAddress();
        if (serverAddress == null) {
            return;
        }

        final String serverPort = inputServerPort();
        if (serverPort == null) {
            return;
        }

        final String userName = inputUserName();
        if (userName == null) {
            return;
        }

        final Protocol protocol = inputProtocol();
        if (protocol == null) {
            return;
        }

        final ChatUser chatUser = new ChatUser(userName, protocol.name());
        client.join(Integer.parseInt(serverPort), serverAddress, chatUser, protocol);
    }

    @Override
    public void actionPerformed(ActionEvent actionEvent) {
        client.sendChatMessage();
    }

    @Override
    public void windowClosing(WindowEvent windowEvent) {
        client.leave();
        client.shutdown();
    }

    private String inputServerAddress() {
        return inputValue(view::inputServerAddress, (value) -> !value.isBlank() && Utils.isAddressValid(value));
    }

    private String inputServerPort() {
        return inputValue(view::inputPort, (value) -> !value.isBlank() && Utils.isPortValid(value));
    }

    private String inputUserName() {
        return inputValue(view::inputUserName, (value) -> !value.isBlank());
    }

    private Protocol inputProtocol() {
        return inputValue(view::inputProtocol, (value) -> true);
    }

    private <T> T inputValue(Supplier<T> inputFunction, Predicate<T> isValidPredicate) {
        T ret;
        do {
            ret = inputFunction.get();

            if (ret == null) {
                view.close();
                client.shutdown();
                return null;
            }
        } while (!isValidPredicate.test(ret));

        return ret;
    }

    @Override
    public void keyPressed(KeyEvent keyEvent) {
        if (keyEvent.getKeyCode() == KeyEvent.VK_CONTROL) {
            ctrlReleased = false;
        }

        if (keyEvent.getKeyCode() != KeyEvent.VK_ENTER) {
            return;
        }

        if (ctrlReleased) {
            client.sendChatMessage();
        } else {
            client.insertLineBreak();
        }
    }

    @Override
    public void keyTyped(KeyEvent keyEvent) {
    }

    @Override
    public void keyReleased(KeyEvent keyEvent) {
        if (keyEvent.getKeyCode() == KeyEvent.VK_CONTROL) {
            ctrlReleased = true;
        }
    }
}
