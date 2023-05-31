package ru.nsu.kondrenko.common.protocols.xml;

import ru.nsu.kondrenko.common.Message;
import ru.nsu.kondrenko.common.protocols.MessageOutputStream;
import ru.nsu.kondrenko.common.protocols.MessageStreamException;

import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;

class XMLMessageOutputStream implements MessageOutputStream {
    private static final int BYTES_IN_INTEGER = 4;
    private final OutputStream outputStream;

    public XMLMessageOutputStream(OutputStream outputStream) {
        this.outputStream = outputStream;
    }

    private static byte[] intToBytes(int number) {
        return ByteBuffer.allocate(BYTES_IN_INTEGER).putInt(number).array();
    }

    @Override
    public void writeMessage(Message message) throws MessageStreamException {
        try {
            final String xmlMessage = message.toXML();
            final int messageLengthInBytes = xmlMessage.getBytes(StandardCharsets.UTF_8).length;
            outputStream.write(intToBytes(messageLengthInBytes));
            outputStream.write(xmlMessage.getBytes(StandardCharsets.UTF_8));
        } catch (Exception exception) {
            throw new MessageStreamException(exception);
        }
    }

    @Override
    public void flush() throws MessageStreamException {
        try {
            outputStream.flush();
        } catch (Exception exception) {
            throw new MessageStreamException(exception);
        }
    }

    @Override
    public void close() throws MessageStreamException {
        try {
            outputStream.close();
        } catch (Exception exception) {
            throw new MessageStreamException(exception);
        }
    }
}
