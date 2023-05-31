package ru.nsu.kondrenko.client.model.messagedocument;

public interface MessageDocument {
    String getContent();

    void clearContent();

    void insertLineBreak();
}
