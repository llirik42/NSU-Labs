package ru.nsu.kondrenko.client;

import net.sourceforge.argparse4j.ArgumentParsers;
import net.sourceforge.argparse4j.inf.ArgumentParser;
import net.sourceforge.argparse4j.inf.Namespace;

import java.io.File;

public class Main {
    public static void main(String[] args) {
        ArgumentParser argumentParser = ArgumentParsers.newFor("Программа").build();

        argumentParser.addArgument("-p", "--port").required(true).help("Порт сервера").type(Integer.class);
        argumentParser.addArgument("-s", "--server").required(true).help("Адрес или имя сервера").type(String.class);
        argumentParser.addArgument("-f", "--file").required(true).help("Файл для отправки").type(String.class);

        final Namespace argumentsNamespace = argumentParser.parseArgsOrFail(args);

        final int port = argumentsNamespace.getInt("port");
        final String server = argumentsNamespace.getString("server");
        final String fileName = argumentsNamespace.getString("file");

        final File file = new File(fileName);
        if (!file.exists()) {
            System.err.printf("File \"%s\" doesn't exist", fileName);
            return;
        }

        try {
            final Client client = new DefaultClient(port, server, file);
            client.launch();
        } catch (Exception exception) {
            System.err.println(exception.getLocalizedMessage());
        }
    }
}
