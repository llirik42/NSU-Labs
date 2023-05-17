package ru.nsu.kondrenko;

import ru.nsu.kondrenko.controller.Controller;
import ru.nsu.kondrenko.view.View;

public class Main {
    public static void main(String[] args) {
        View view = new View();
        Controller controller = new Controller(view);
        controller.start();
    }
}
