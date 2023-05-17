package ru.nsu.kondrenko.ui;

import ru.nsu.kondrenko.model.PicturesFactory;
import ru.nsu.kondrenko.model.PicturesFactoryImpl;
import ru.nsu.kondrenko.model.PicturesFactoryProxy;

import java.util.Random;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        var scanner = new Scanner(System.in);

        System.out.print("Введите K: ");
        int K = scanner.nextInt();

        System.out.print("Введите N: ");
        int N = scanner.nextInt();

        PicturesFactory factory = new PicturesFactoryImpl();

        PicturesFactory proxy = new PicturesFactoryProxy(N, factory);

        Random random = new Random();
        for (int i = 0; i < K; i++){
            var currentId = random.nextInt() % 3;

            var currentPicture = proxy.getPicture(currentId);

            System.out.println(currentPicture);
        }
    }
}
