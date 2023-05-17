package ru.nsu.kondrenko;

import ru.nsu.kondrenko.controller.Controller;

public class Main {
    public static void main(String[] args) throws InterruptedException {
        Controller controller = new Controller();
        controller.start();
    }
}
