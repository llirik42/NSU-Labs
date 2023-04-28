package ru.nsu.kondrenko.model.statistics;

import ru.nsu.kondrenko.model.exceptions.StatisticStorageException;

public interface StatisticsStorage extends Iterable<PlayerScoreRecord> {
    void fetchRecords() throws StatisticStorageException;

    void storeRecords() throws StatisticStorageException;

    int getCountOfRecords();

    PlayerScoreRecord getRecord(int index);

    void addWinner(String winnerName);
}
