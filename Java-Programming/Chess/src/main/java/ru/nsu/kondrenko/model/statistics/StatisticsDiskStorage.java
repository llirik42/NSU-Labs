package ru.nsu.kondrenko.model.statistics;

import ru.nsu.kondrenko.model.exceptions.StatisticStorageException;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.*;

public class StatisticsDiskStorage implements StatisticsStorage {
    private static final String SCORE_FILE_PATH = "score.txt";

    private final List<PlayerScoreRecord> records = new ArrayList<>();

    @Override
    public void fetchRecords() throws StatisticStorageException {
        final File scoreFile = new File(SCORE_FILE_PATH);

        if (!scoreFile.exists()) {
            return;
        }

        try (final Scanner scanner = new Scanner(scoreFile)) {
            while (scanner.hasNext()) {
                final String line = scanner.nextLine();

                if (line.isBlank() || line.isEmpty()) {
                    continue;
                }

                addIfAbsent(records, extractPlayerScoreRecord(line));
            }
        } catch (FileNotFoundException | IllegalStateException | NoSuchElementException exception) {
            throw new StatisticStorageException("Error during reading the score file", exception);
        } catch (NumberFormatException exception) {
            throw new StatisticStorageException("Incorrect score of player in the score file", exception);
        }
    }

    @Override
    public void storeRecords() throws StatisticStorageException {
        final File scoreFile = new File(SCORE_FILE_PATH);

        try (FileWriter fileWriter = new FileWriter(scoreFile)) {
            for (final var it : records) {
                fileWriter.write(it.playerName() + ' ');
                fileWriter.write(String.valueOf(it.score()));
                fileWriter.write('\n');
            }
        } catch (IOException exception) {
            throw new StatisticStorageException("Error during saving records", exception);
        }
    }

    @Override
    public int getCountOfRecords() {
        return records.size();
    }

    @Override
    public PlayerScoreRecord getRecord(int index) {
        return records.get(index);
    }

    @Override
    public void addWinner(final String winnerName) {
        boolean contained = false;

        for (final var it : records) {
            if (!winnerName.equals(it.playerName())) {
                continue;
            }

            final int oldScore = it.score();
            records.remove(it);
            records.add(new PlayerScoreRecord(winnerName, oldScore + 1));
            contained = true;
            break;
        }

        if (!contained) {
            records.add(new PlayerScoreRecord(winnerName, 1));
        }
    }

    @Override
    public Iterator<PlayerScoreRecord> iterator() {
        return records.iterator();
    }

    private static PlayerScoreRecord extractPlayerScoreRecord(final String String) throws StatisticStorageException {
        final String[] values = String.strip().split(" ");

        if (values.length != 2) {
            throw new StatisticStorageException("Incorrect format of score file");
        }

        final String playerName = values[0].strip().replaceAll(" ", "");
        final int playerScore = Integer.parseInt(values[1]);

        return new PlayerScoreRecord(playerName, playerScore);
    }

    private static <E> void addIfAbsent(final List<E> list, final E element) {
        if (!list.contains(element)) {
            list.add(element);
        }
    }
}
