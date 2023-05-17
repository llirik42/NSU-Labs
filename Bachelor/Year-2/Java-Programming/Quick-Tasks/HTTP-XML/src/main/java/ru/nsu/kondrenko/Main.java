package ru.nsu.kondrenko;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import java.io.IOException;
import java.io.InputStream;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.List;

public class Main {
    private static final String LINK = "https://habr.com/ru/rss/best/daily";

    public static void main(String[] args) throws IOException, ParserConfigurationException, SAXException {
        DocumentBuilder builder = DocumentBuilderFactory.newDefaultInstance().newDocumentBuilder();

        final URL url = new URL(LINK);

        final InputStream inputStream = url.openConnection().getInputStream();
        Document document = builder.parse(inputStream);

        NodeList nodeList = document.getElementsByTagName("item");

        for (int i = 0; i < nodeList.getLength(); i++) {
            final Node node = nodeList.item(i);

            final String content = node.getTextContent();

            final List<String> lines = content.lines().toList();

            final String title = lines.get(1).strip();
            final String link = lines.get(2).strip();

            System.out.printf("%s    %s\n\n", title, link);
        }
    }
}
