package ru.nsu.kondrenko.view;

import ru.nsu.kondrenko.model.Listener;
import ru.nsu.kondrenko.model.Record;
import ru.nsu.kondrenko.model.StorageRecord;

public class View implements Listener {
    @Override
    public void notifyConsumption(String consumerName, String product) {
        System.out.println(consumerName +  " consumes " + product);
    }

    public void showProducerTermination(Record record) {
        System.out.println(record.name() + " produced " + record.doneCount());
    }

    public void showAllProducersTermination() {
        System.out.println("\nProducers are terminated\n");
    }

    public void showConsumerTermination(Record record) {
        System.out.println(record.name() + " consumed " + record.doneCount());
    }

    public void showAllConsumersTermination() {
        System.out.println("\nConsumers are terminated\n");
    }

    public void showEmptyStorage() {
        System.out.println("\nThe storage is empty");
    }

    public void showStorageTermination(StorageRecord record) {
        System.out.println("\n" + record.name() + "\n  placed: " + record.placedCount() + "\n  taken: " + record.placedCount());
    }
}
