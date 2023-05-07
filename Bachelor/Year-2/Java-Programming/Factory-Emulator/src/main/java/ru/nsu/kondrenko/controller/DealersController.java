package ru.nsu.kondrenko.controller;

import ru.nsu.kondrenko.model.Emulator;

import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

public class DealersController implements ChangeListener {
    private final Emulator emulator;

    public DealersController(Emulator emulator) {
        this.emulator = emulator;
    }

    @Override
    public void stateChanged(ChangeEvent changeEvent) {
        emulator.setDealersCount(Utils.getValueFromSliderEvent(changeEvent));
    }
}
