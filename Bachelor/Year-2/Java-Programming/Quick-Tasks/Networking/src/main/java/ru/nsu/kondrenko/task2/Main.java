package ru.nsu.kondrenko.task2;

import java.io.IOException;
import java.io.PrintWriter;
import java.io.Writer;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.HashMap;
import java.util.Map;
import java.util.NoSuchElementException;
import java.util.Scanner;

public class Main {
    private static final int HTTP_PORT = 80;
    private static final int HTTP_SUCCESS = 200;

    public static void main(String[] args) throws IOException {
        if (args.length == 0 || args.length > 2) {
            System.err.println("Incorrect number of args!\n");
        }

        final String host = args[0];
        final String resource = args.length == 2 ? "/" + args[1] : "/";

        try (final Socket socket = new Socket(host, HTTP_PORT);
             final Scanner scanner = new Scanner(socket.getInputStream());
             final Writer writer = new PrintWriter(socket.getOutputStream())) {

            final String request = String.format("GET %s HTTP/1.0\n\n", resource);

            writer.write(request);
            writer.flush();

            final String responseLine = scanner.nextLine();
            final String responseCode = responseLine.split(" ")[1];

            if (Integer.parseInt(responseCode) != HTTP_SUCCESS) {
                System.err.println("Failure!\n");
            } else {
                final Map<String, String> values = new HashMap<>();

                while (scanner.hasNext()) {
                    final String line = scanner.nextLine();

                    if (line.isBlank()) {
                        break;
                    }

                    final String[] parts = line.split(":");

                    values.put(parts[0], parts[1]);
                }

                for (var it : values.entrySet()) {
                    System.out.printf("%s:%s%n", it.getKey(), it.getValue());
                }
            }
        } catch (UnknownHostException exception) {
            System.err.println("Unknown host!\n");
        } catch (NumberFormatException | NoSuchElementException | IllegalStateException exception) {
            System.err.println("Host respond incorrectly!\n");
        }
    }
}
