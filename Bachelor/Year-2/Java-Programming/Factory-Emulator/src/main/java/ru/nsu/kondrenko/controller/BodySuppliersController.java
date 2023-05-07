package ru.nsu.kondrenko.controller;

import ru.nsu.kondrenko.model.Emulator;

import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

public class BodySuppliersController implements ChangeListener {
    private final Emulator emulator;

    public BodySuppliersController(Emulator emulator) {
        this.emulator = emulator;
    }

    @Override
    public void stateChanged(ChangeEvent changeEvent) {
        emulator.setBodySuppliersCount(Utils.getValueFromSliderEvent(changeEvent));
    }
}
