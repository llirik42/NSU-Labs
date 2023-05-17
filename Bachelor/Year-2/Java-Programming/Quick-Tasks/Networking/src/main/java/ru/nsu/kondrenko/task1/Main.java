package ru.nsu.kondrenko.task1;

import java.io.*;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Scanner;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Main {
    private static final int PORT = 8080;
    private static final int BACKLOG = 50;
    private static final String PATH_TO_FILES_DIRECTORY = "server_files/";
    private static final int BUFFER_SIZE = 1024;
    private static final int THREADS_COUNT = 8;

    public static void main(String[] args) {
        try (final ExecutorService executorService = Executors.newFixedThreadPool(THREADS_COUNT);
             final ServerSocket serverSocket = new ServerSocket(PORT, BACKLOG, InetAddress.getLoopbackAddress())) {

            for (int i = 0; i < THREADS_COUNT; i++) {
                executorService.submit(() -> {
                    try {
                        task(serverSocket);
                    } catch (Exception exception) {
                        throw new RuntimeException(exception);
                    }
                });
            }

            while (!executorService.isShutdown()) {
                synchronized (Thread.currentThread()) {
                    Thread.currentThread().wait();
                }
            }
        } catch (IOException exception) {
            System.err.println("Couldn't open socket!\n");
        } catch (IllegalArgumentException exception) {
            System.err.println("Incorrect port!\n");
        } catch (InterruptedException exception) {
            throw new RuntimeException(exception);
        }
    }

    private static void task(final ServerSocket serverSocket) throws IOException {
        while (true) {
            final Socket socket = serverSocket.accept();

            try (final Scanner scanner = new Scanner(socket.getInputStream());
                 final Writer writer = new PrintWriter(socket.getOutputStream())) {

                final String fileName = scanner.nextLine().split(" ")[1].substring(1);

                File file = new File(String.format("%s%s", PATH_TO_FILES_DIRECTORY, fileName));

                if (!file.exists()) {
                    writer.write("HTTP/1.0 404 Not Found\n\n");
                } else {
                    writer.write("HTTP/1.0 200 OK\n\n");

                    try (final Reader fileReader = new FileReader(file)) {
                        final char[] buffer = new char[BUFFER_SIZE];

                        int readCount = fileReader.read(buffer);
                        final int eof = -1;
                        while (readCount != eof) {
                            writer.write(buffer, 0, readCount);
                            readCount = fileReader.read(buffer);
                        }
                    }
                }
            }
        }
    }
}
