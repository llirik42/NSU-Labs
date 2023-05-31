package ru.nsu.kondrenko.client.view;

import ru.nsu.kondrenko.client.model.ClientListener;
import ru.nsu.kondrenko.client.model.messagedocument.MessageDocument;
import ru.nsu.kondrenko.common.ClientContext;
import ru.nsu.kondrenko.common.modelobjects.ChatMessage;
import ru.nsu.kondrenko.common.modelobjects.ChatUser;
import ru.nsu.kondrenko.common.protocols.Protocol;

import javax.swing.*;
import javax.swing.text.DefaultCaret;
import java.awt.*;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.WindowListener;
import java.util.List;

public class SwingView implements ClientListener {
    private static final Font GLOBAL_FONT = new Font(Font.SERIF, Font.PLAIN, 24);
    private static final Dimension PREFERRED_SIZE = new Dimension(1200, 600);
    private static final int BOTTOM_PANEL_HEIGHT = 150;
    private static final int EAST_SIDE_PREFERRED_WIDTH = 350;

    private final JFrame frame;
    private final JTextArea centralTextArea;
    private final JTextArea usersListTextArea;
    private MessageDocument messageDocument;
    private ClientContext clientContext;
    private boolean isClosing = false;

    public SwingView(WindowListener windowListener, KeyListener keyListener) {
        frame = new JFrame();
        centralTextArea = new JTextArea();
        centralTextArea.setFont(GLOBAL_FONT);
        centralTextArea.setEditable(false);
        centralTextArea.setLineWrap(true);
        centralTextArea.setWrapStyleWord(true);
        ((DefaultCaret) centralTextArea.getCaret()).setUpdatePolicy(DefaultCaret.ALWAYS_UPDATE);

        frame.setLayout(new BorderLayout());
        frame.setPreferredSize(PREFERRED_SIZE);

        final JScrollPane jScrollPane = new JScrollPane(centralTextArea);
        jScrollPane.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
        frame.add(jScrollPane, BorderLayout.CENTER);
        frame.add(createBottomPanel(keyListener), BorderLayout.SOUTH);
        frame.addWindowListener(windowListener);

        usersListTextArea = new JTextArea();
        usersListTextArea.setFont(GLOBAL_FONT);
        usersListTextArea.setPreferredSize(new Dimension(EAST_SIDE_PREFERRED_WIDTH, 0));
        frame.add(new JScrollPane(usersListTextArea), BorderLayout.EAST);
        frame.pack();

        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
    }

    public void setClientContext(ClientContext clientContext) {
        this.clientContext = clientContext;
    }

    private JPanel createBottomPanel(KeyListener keyListener) {
        final JTextArea newMessageArea = new JTextArea();
        newMessageArea.setFont(GLOBAL_FONT);
        newMessageArea.setLineWrap(true);
        newMessageArea.setWrapStyleWord(true);
        newMessageArea.addKeyListener(keyListener);
        newMessageArea.getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, 0), "None");

        newMessageArea.getCaret().getMagicCaretPosition();
        messageDocument = new SwingMessageDocument(newMessageArea);

        final JPanel ret = new JPanel();
        ret.setLayout(new GridLayout(1, 2));
        ret.add(new JScrollPane(newMessageArea));
        ret.setPreferredSize(new Dimension(0, BOTTOM_PANEL_HEIGHT));

        return ret;
    }

    public String inputServerAddress() {
        return inputValue("Enter address of server", "Address");
    }

    public String inputPort() {
        return inputValue("Enter port of server", "Port");
    }

    public String inputUserName() {
        return inputValue("Enter username", "Username");
    }

    public Protocol inputProtocol() {
        final Protocol[] possibilities = Protocol.values();
        return (Protocol) JOptionPane.showInputDialog(
                frame,
                "Choose protocol",
                "Protocol",
                JOptionPane.PLAIN_MESSAGE,
                null,
                possibilities,
                Protocol.values()[0]);
    }

    public MessageDocument getMessageDocument() {
        return messageDocument;
    }

    public void close() {
        isClosing = true;

        SwingUtilities.invokeLater(() -> {
            frame.setVisible(false);
            frame.dispose();
        });
    }

    private String inputValue(String message, String title) {
        final String ret = JOptionPane.showInputDialog(frame, message, title, JOptionPane.QUESTION_MESSAGE);

        return ret == null
                ? null
                : ret.strip();
    }

    @Override
    public void onClientDisconnect() {
        close();
    }

    @Override
    public void onException(Exception exception) {
        showError(exception.getLocalizedMessage());
    }

    @Override
    public void onLogin(ChatUser chatUser) {
        centralTextArea.append(String.format("%s joined%n%n", chatUser.name()));
        updateChatUsersList();
    }

    @Override
    public void onLogout(ChatUser chatUser) {
        centralTextArea.append(String.format("%s left%n%n", chatUser.name()));
        updateChatUsersList();
    }

    @Override
    public void onErrorResponse(String message) {
        showError(message);
    }

    @Override
    public void onChatMessage(ChatMessage chatMessage) {
        centralTextArea.append(chatMessage.sender().name() + ":\n");
        centralTextArea.append(chatMessage.content() + "\n\n");
    }

    @Override
    public void onUsersListUpdate(List<ChatUser> chatUsersList) {
        updateChatUsersList();
    }

    private void showError(String message) {
        if (!isClosing) {
            SwingUtilities.invokeLater(() -> JOptionPane.showMessageDialog(
                    null,
                    message,
                    "Error",
                    JOptionPane.ERROR_MESSAGE
            ));
        }
    }

    private void updateChatUsersList() {
        usersListTextArea.setText("");
        for (final var it : clientContext.getChatUsersList()) {
            usersListTextArea.append(String.format("%s%n%n", it.name()));
        }
    }
}
