package ru.nsu.kondrenko.controller;

import ru.nsu.kondrenko.model.Emulator;

import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

public class WindowClosingController extends WindowAdapter {
    private final Emulator emulator;

    public WindowClosingController(Emulator emulator) {
        this.emulator = emulator;
    }

    @Override
    public void windowClosing(WindowEvent windowEvent) {
        emulator.terminate();
    }
}
