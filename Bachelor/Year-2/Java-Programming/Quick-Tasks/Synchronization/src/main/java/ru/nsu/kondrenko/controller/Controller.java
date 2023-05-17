package ru.nsu.kondrenko.controller;

import ru.nsu.kondrenko.model.Listener;
import ru.nsu.kondrenko.model.RestRoom;
import ru.nsu.kondrenko.model.Utils;
import ru.nsu.kondrenko.view.View;

import java.util.Scanner;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Controller {
    private static final int WORK_MIN_TIME = 50;
    private static final int WORK_MAX_TIME = 100;

    private final Scanner scanner = new Scanner(System.in);

    public void start() throws InterruptedException {
        Listener view = new View();

        final int n = inputNumber("N", scanner);
        final int k = inputNumber("K", scanner);
        final int t = inputNumber("T", scanner);

        RestRoom restRoom = new RestRoom(view, k);

        try (ExecutorService workers = Executors.newFixedThreadPool(n)) {
            for (int i = 0; i < n; i++) {
                workers.submit(() -> {
                    while (!Thread.interrupted()) {
                        try {
                            Thread.sleep(Utils.generateRandomNumber(WORK_MIN_TIME, WORK_MAX_TIME));
                            restRoom.enterAndRest();
                        } catch (InterruptedException e) {
                            throw new RuntimeException(e);
                        }
                    }
                });
            }

            Thread.sleep(t * 1000L);

            workers.shutdownNow();
        }
    }

    private static int inputNumber(String name, Scanner scanner) {
        System.out.print(name +  ": ");
        return scanner.nextInt();
    }
}
