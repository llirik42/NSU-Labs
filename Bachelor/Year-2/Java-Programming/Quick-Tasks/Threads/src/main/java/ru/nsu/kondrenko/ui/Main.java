package ru.nsu.kondrenko.ui;

import ru.nsu.kondrenko.model.Consumer;
import ru.nsu.kondrenko.model.Producer;
import ru.nsu.kondrenko.model.Storage;

import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) throws InterruptedException {
        final Scanner scanner = new Scanner(System.in);

        final int n = inputNumber("N", scanner);
        final int p = inputNumber("P", scanner);
        final int c = inputNumber("C", scanner);
        final int t = inputNumber("T", scanner);

        final Storage<String> storage = new Storage<>(n);

        final List<Producer> producers = new ArrayList<>();
        for (int i = 0; i < p; i++) {
            Producer producer = new Producer(storage);
            producer.start();
            producers.add(producer);
        }

        final List<Consumer> consumers = new ArrayList<>();
        for (int i = 0; i < c; i++) {
            Consumer consumer = new Consumer(storage);
            consumer.start();
            consumers.add(consumer);
        }

        Thread.sleep(t * 1000L);

        System.out.print("\n");
        for (Producer producer : producers) {
            producer.interrupt();
            producer.join();
        }

        System.out.println("\nPRODUCERS ARE TERMINATED");

        synchronized (storage) {
            while (!storage.isEmpty()) {
                storage.wait();
            }
        }

        System.out.println("\nTHE STORAGE IS EMPTY\n");

        for (Consumer consumer : consumers) {
            consumer.interrupt();
            consumer.join();
        }

        System.out.println("\nCONSUMERS ARE TERMINATED\n");

        final int storagePlacedCount = storage.getPlacedCount();
        final int storageTakenCount = storage.getTakenCount();

        System.out.println("Storage\n  placed: " + storagePlacedCount + "\n  taken: " + storageTakenCount);
    }

    private static int inputNumber(String name, Scanner scanner) {
        System.out.print(name +  ": ");
        return scanner.nextInt();
    }
}
