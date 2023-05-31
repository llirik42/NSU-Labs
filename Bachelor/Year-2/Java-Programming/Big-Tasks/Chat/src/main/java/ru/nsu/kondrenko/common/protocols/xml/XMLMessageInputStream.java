package ru.nsu.kondrenko.common.protocols.xml;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import ru.nsu.kondrenko.common.Message;
import ru.nsu.kondrenko.common.messages.*;
import ru.nsu.kondrenko.common.modelobjects.ChatMessage;
import ru.nsu.kondrenko.common.modelobjects.ChatUser;
import ru.nsu.kondrenko.common.modelobjects.Session;
import ru.nsu.kondrenko.common.protocols.MessageInputStream;
import ru.nsu.kondrenko.common.protocols.MessageStreamException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import java.io.IOException;
import java.io.InputStream;
import java.io.StringReader;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

class XMLMessageInputStream implements MessageInputStream {
    private static final int BYTES_IN_INTEGER = 4;
    private static final DocumentBuilder documentBuilder;
    private static final String DEFAULT_USER_TYPE = "XML";

    static {
        try {
            documentBuilder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
        } catch (ParserConfigurationException exception) {
            throw new RuntimeException(exception);
        }
    }

    private final InputStream inputStream;

    public XMLMessageInputStream(InputStream inputStream) {
        this.inputStream = inputStream;
    }

    private static ChatUser parseChatUser(Node node) throws InvalidMessageException {
        final String name = parseField(node, "name");
        final String type = parseField(node, "type");
        return new ChatUser(name, type);
    }

    private static Session parseSession(Node node) {
        return new Session(parseValueOfNode(node));
    }

    private static ChatMessage parseChatMessage(Node node) throws InvalidMessageException {
        final String content = parseField(node, "message");
        final String name = parseField(node, "name");
        return new ChatMessage(content, new ChatUser(name, DEFAULT_USER_TYPE));
    }

    private static String parseValueOfNode(Node node) {
        return node.getTextContent().strip();
    }

    private static String parseField(Node node, String fieldName) throws InvalidMessageException {
        final NodeList childNodes = node.getChildNodes();
        final int childNodesLength = childNodes.getLength();
        for (int i = 0; i < childNodesLength; i++) {
            final Node childNode = childNodes.item(i);
            final String childNodeName = childNode.getNodeName();

            if (childNodeName.equals(fieldName)) {
                return parseValueOfNode(childNode);
            }
        }

        throw new InvalidMessageException();
    }

    @Override
    public Message readMessage() throws MessageStreamException, InvalidMessageException {
        String xmlMessage = null;

        try {
            final byte[] bytes = new byte[BYTES_IN_INTEGER];
            final int readCount = inputStream.read(bytes);

            if (readCount != BYTES_IN_INTEGER) {
                throw new MessageStreamException("Cannot read from socket");
            }

            final int messageLengthInBytes = ByteBuffer.wrap(bytes).getInt();
            if (messageLengthInBytes <= 0) {
                throw new InvalidMessageException("Incorrect length of XML-message");
            }

            xmlMessage = new String(inputStream.readNBytes(messageLengthInBytes), StandardCharsets.UTF_8);
            final InputSource inputSource = new InputSource(new StringReader(xmlMessage));
            return parseMessage(documentBuilder.parse(inputSource));
        } catch (InvalidMessageException exception) {
            throw new InvalidMessageException(exception, xmlMessage);
        } catch (SAXException exception) {
            throw new InvalidMessageException("Invalid format of XML-message", exception);
        } catch (IOException exception) {
            throw new MessageStreamException(exception);
        }
    }

    @Override
    public void close() throws MessageStreamException {
        try {
            inputStream.close();
        } catch (Exception exception) {
            throw new MessageStreamException(exception);
        }
    }

    private Message parseMessage(Document document) throws InvalidMessageException {
        final String rootTag = document.getDocumentElement().getTagName();

        if (Objects.equals(rootTag, "command")) {
            return parseCommand(document);
        }
        if (Objects.equals(rootTag, "event")) {
            return parseEvent(document);
        }
        if (Objects.equals(rootTag, "error")) {
            return parseError(document);
        }
        if (Objects.equals(rootTag, "success")) {
            return parseSuccess(document);
        }

        throw new InvalidMessageException("Unknown root tag of XML-message");
    }

    private Message parseCommand(Document document) throws InvalidMessageException {
        final Node attributeNode = document.getDocumentElement().getAttributeNode("name");

        if (attributeNode == null) {
            throw new InvalidMessageException();
        }

        final String attributeValue = attributeNode.getNodeValue();
        final Node rootNode = document.getDocumentElement();

        if (Objects.equals(attributeValue, "login")) {
            return new LoginRequest(parseChatUser(rootNode));
        }
        if (Objects.equals(attributeValue, "list")) {
            return new UsersListRequest(parseSession(rootNode));
        }
        if (Objects.equals(attributeValue, "message")) {
            final Session session = new Session(parseField(rootNode, "session"));
            final String messageContent = parseField(rootNode, "message");
            return new MessageSendingRequest(session, messageContent);
        }
        if (Objects.equals(attributeValue, "logout")) {
            return new LogoutRequest(parseSession(rootNode));
        }

        throw new InvalidMessageException();
    }

    private Message parseEvent(Document document) throws InvalidMessageException {
        final Node attributeNode = document.getDocumentElement().getAttributeNode("name");

        if (attributeNode == null) {
            throw new InvalidMessageException();
        }

        final String attributeValue = attributeNode.getNodeValue();
        final Node rootNode = document.getDocumentElement();

        if (Objects.equals(attributeValue, "message")) {
            return new ChatMessageEvent(parseChatMessage(rootNode));
        }
        if (Objects.equals(attributeValue, "userlogin")) {
            final ChatUser chatUser = new ChatUser(parseValueOfNode(rootNode), DEFAULT_USER_TYPE);
            return new LoginEvent(chatUser);
        }
        if (Objects.equals(attributeValue, "userlogout")) {
            final ChatUser chatUser = new ChatUser(parseValueOfNode(rootNode), DEFAULT_USER_TYPE);
            return new LogoutEvent(chatUser);
        }

        throw new InvalidMessageException();

    }

    private Message parseError(Document document) throws InvalidMessageException {
        return new ErrorResponse(parseField(document.getDocumentElement(), "message"));
    }

    private Message parseSuccess(Document document) throws InvalidMessageException {
        final NodeList sessionNodes = document.getDocumentElement().getElementsByTagName("session");
        final NodeList listuserNodes = document.getDocumentElement().getElementsByTagName("listusers");

        if (sessionNodes.getLength() == 0 && listuserNodes.getLength() == 0) {
            return new SuccessResponse();
        }
        if (sessionNodes.getLength() == 1) {
            final Session session = parseSession(sessionNodes.item(0));
            return new LoginResponse(session);
        }
        if (listuserNodes.getLength() == 1) {
            final NodeList userNodes = document.getDocumentElement().getElementsByTagName("user");
            final List<ChatUser> chatUsersList = new ArrayList<>();

            final int length = userNodes.getLength();
            for (int i = 0; i < length; i++) {
                final Node node = userNodes.item(i);
                chatUsersList.add(parseChatUser(node));
            }

            return new UsersListResponse(chatUsersList);
        }

        throw new InvalidMessageException();
    }
}
