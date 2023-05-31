package ru.nsu.kondrenko.common.protocols;

import java.io.InputStream;
import java.io.OutputStream;

public interface MessageStreamsFactory {
    MessageInputStream createMessageInputStream(InputStream inputStream) throws MessageStreamException;

    MessageOutputStream createMessageOutputStream(OutputStream outputStream) throws MessageStreamException;
}
