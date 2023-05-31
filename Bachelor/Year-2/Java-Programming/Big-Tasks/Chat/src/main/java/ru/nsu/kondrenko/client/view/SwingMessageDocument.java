package ru.nsu.kondrenko.client.view;

import ru.nsu.kondrenko.client.model.messagedocument.MessageDocument;

import javax.swing.*;
import javax.swing.text.Document;

public class SwingMessageDocument implements MessageDocument {
    private final JTextArea jTextArea;
    private final Document document;

    public SwingMessageDocument(JTextArea jTextArea) {
        this.jTextArea = jTextArea;
        this.document = jTextArea.getDocument();
    }

    @Override
    public String getContent() {
        try {
            return document.getText(0, document.getLength());
        } catch (Exception ignored) {
            return null;
        }
    }

    @Override
    public void clearContent() {
        try {
            document.remove(0, document.getLength());
        } catch (Exception ignored) {
        }
    }

    @Override
    public void insertLineBreak() {
        try {
            document.insertString(jTextArea.getCaretPosition(), "\n", null);
        } catch (Exception ignored) {
        }
    }
}
