package ru.nsu.kondrenko.view;

import ru.nsu.kondrenko.controller.SwingController;
import ru.nsu.kondrenko.model.Game;
import ru.nsu.kondrenko.model.Listener;

import javax.swing.*;
import java.awt.*;

public class SwingViewer extends JFrame implements Listener {
    private final JLabel messageLabel;
    private final JLabel taskLabel;

    public SwingViewer(Game game) {
        setLayout(new FlowLayout());

        this.messageLabel = new JLabel();
        this.taskLabel =  new JLabel();
        JButton button = new JButton();
        JTextField textField = new JTextField("", 10);
        SwingController controller = new SwingController(game, textField.getDocument());

        button.addActionListener(controller);

        textField.addActionListener(controller);

        add(taskLabel);
        add(textField);
        add(button);
        add(messageLabel);

        pack();
    }

    @Override
    public void notifyInputMessage(int number) {
        taskLabel.setText("You have to write [sqrt(" + number + ")]");
    }

    @Override
    public void notifyCheck(boolean match) {
        String message = match ? "You calculated correctly" : "You calculated incorrectly";

        messageLabel.setText(message);
    }

    @Override
    public void notifyIncorrectFormat() {
        messageLabel.setText("You can input only numbers");
    }

    @Override
    public void notifyYouAreLate() {
        messageLabel.setText("You are late");
    }

    @Override
    public void notifyGameOver() {
        setVisible(false);
        dispose();
    }
}
