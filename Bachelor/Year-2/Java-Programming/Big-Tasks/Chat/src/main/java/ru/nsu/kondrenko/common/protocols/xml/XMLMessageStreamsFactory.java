package ru.nsu.kondrenko.common.protocols.xml;

import ru.nsu.kondrenko.common.protocols.MessageInputStream;
import ru.nsu.kondrenko.common.protocols.MessageOutputStream;
import ru.nsu.kondrenko.common.protocols.MessageStreamsFactory;

import java.io.InputStream;
import java.io.OutputStream;

public class XMLMessageStreamsFactory implements MessageStreamsFactory {
    @Override
    public MessageInputStream createMessageInputStream(InputStream inputStream) {
        return new XMLMessageInputStream(inputStream);
    }

    @Override
    public MessageOutputStream createMessageOutputStream(OutputStream outputStream) {
        return new XMLMessageOutputStream(outputStream);
    }
}
