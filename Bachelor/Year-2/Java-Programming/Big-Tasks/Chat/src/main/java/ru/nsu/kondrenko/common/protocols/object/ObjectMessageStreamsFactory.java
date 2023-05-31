package ru.nsu.kondrenko.common.protocols.object;

import ru.nsu.kondrenko.common.protocols.MessageInputStream;
import ru.nsu.kondrenko.common.protocols.MessageOutputStream;
import ru.nsu.kondrenko.common.protocols.MessageStreamException;
import ru.nsu.kondrenko.common.protocols.MessageStreamsFactory;

import java.io.InputStream;
import java.io.OutputStream;

public class ObjectMessageStreamsFactory implements MessageStreamsFactory {
    @Override
    public MessageInputStream createMessageInputStream(InputStream inputStream) throws MessageStreamException {
        return new ObjectMessageInputStream(inputStream);
    }

    @Override
    public MessageOutputStream createMessageOutputStream(OutputStream outputStream) throws MessageStreamException {
        return new ObjectMessageOutputStream(outputStream);
    }
}
