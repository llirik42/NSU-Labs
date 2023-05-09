package ru.nsu.kondrenko.controller;

import ru.nsu.kondrenko.model.Emulator;

import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.logging.Logger;

public class WindowClosingController extends WindowAdapter {
    private final Emulator emulator;

    private final Logger logger;

    public WindowClosingController(Emulator emulator, Logger logger) {
        this.emulator = emulator;
        this.logger = logger;
    }

    @Override
    public void windowClosing(WindowEvent windowEvent) {
        emulator.terminate();

        for (var it : logger.getHandlers()) {
            it.flush();
        }
    }
}
