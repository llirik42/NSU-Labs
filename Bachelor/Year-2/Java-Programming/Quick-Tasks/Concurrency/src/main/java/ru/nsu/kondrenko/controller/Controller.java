package ru.nsu.kondrenko.controller;

import ru.nsu.kondrenko.model.Consumer;
import ru.nsu.kondrenko.model.Producer;
import ru.nsu.kondrenko.model.Storage;
import ru.nsu.kondrenko.model.Record;
import ru.nsu.kondrenko.view.View;

import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class Controller {
    private final Scanner scanner = new Scanner(System.in);

    private final View view;

    public Controller(View view) {
        this.view = view;
    }

    public void start() {
        final int n = inputNumber("N", scanner);
        final int p = inputNumber("P", scanner);
        final int c = inputNumber("C", scanner);
        final int t = inputNumber("T", scanner);

        final Storage<String> storage = new Storage<>(n);

        try (ExecutorService producers = Executors.newFixedThreadPool(p);
             ExecutorService consumers = Executors.newFixedThreadPool(c)) {

            List<Future<Record>> producersFuture = new LinkedList<>();
            List<Future<Record>> consumersFuture = new LinkedList<>();

            for (int i = 0; i < p; i++) {
                producersFuture.add(producers.submit(new Producer(storage)));
            }

            for (int i = 0; i < c; i++) {
                consumersFuture.add(consumers.submit(new Consumer(storage, view)));
            }

            Thread.sleep(t * 1000L);

            producers.shutdownNow();

            for (var future : producersFuture) {
                future.get();
            }

            view.showAllProducersTermination();

            storage.startTermination();

            storage.waitUntilEmpty();

            view.showEmptyStorage();

            consumers.shutdownNow();

            for (var future : consumersFuture) {
                future.get();
            }

            view.showAllConsumersTermination();

            for (var future : producersFuture) {
                view.showProducerTermination(future.get());
            }
            for (var future : consumersFuture) {
                view.showConsumerTermination(future.get());
            }

            view.showStorageTermination(storage.getRecord());
        } catch (InterruptedException | ExecutionException exception) {
            throw new RuntimeException(exception);
        }
    }

    private static int inputNumber(String name, Scanner scanner) {
        System.out.print(name +  ": ");
        return scanner.nextInt();
    }
}
