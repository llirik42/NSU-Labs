package ru.nsu.kondrenko.controller;

import ru.nsu.kondrenko.model.Emulator;

import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

public class EngineSuppliersController implements ChangeListener {
    private final Emulator emulator;

    public EngineSuppliersController(Emulator emulator) {
        this.emulator = emulator;
    }

    @Override
    public void stateChanged(ChangeEvent changeEvent) {
        emulator.setEngineSuppliersCount(Utils.getValueFromSliderEvent(changeEvent));
    }
}
