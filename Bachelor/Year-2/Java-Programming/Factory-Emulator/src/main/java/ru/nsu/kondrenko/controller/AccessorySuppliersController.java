package ru.nsu.kondrenko.controller;

import ru.nsu.kondrenko.model.Emulator;

import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

public class AccessorySuppliersController implements ChangeListener {
    private final Emulator emulator;

    public AccessorySuppliersController(Emulator emulator) {
        this.emulator = emulator;
    }

    @Override
    public void stateChanged(ChangeEvent changeEvent) {
        emulator.setAccessorySuppliersCount(Utils.getValueFromSliderEvent(changeEvent));
    }
}
