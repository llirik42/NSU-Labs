package ru.nsu.kondrenko.model;

import java.util.Random;

public class Picture {
    private static final int BYTES_COUNT = 1024 * 1024;

    private final int id;

    private int bytesSum;

    public Picture(int id) {
        this.id = id;
        this.bytesSum = 0;

        Random random = new Random();
        byte[] bytes = new byte[BYTES_COUNT];

        for (int i = 0; i < BYTES_COUNT; i++){
            bytes[i] = (byte) random.nextInt();
            bytesSum += bytes[i];
        }
    }

    @Override
    public String toString() {
        return "Picture{" +
                "id=" + id +
                ", bytesSum=" + bytesSum +
                '}';
    }
}
