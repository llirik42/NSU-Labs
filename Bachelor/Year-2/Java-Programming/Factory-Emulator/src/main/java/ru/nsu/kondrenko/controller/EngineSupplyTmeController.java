package ru.nsu.kondrenko.controller;

import ru.nsu.kondrenko.model.Emulator;

import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

public class EngineSupplyTmeController implements ChangeListener {
    private final Emulator emulator;

    public EngineSupplyTmeController(Emulator emulator) {
        this.emulator = emulator;
    }

    @Override
    public void stateChanged(ChangeEvent changeEvent) {
        emulator.setEngineSupplyTime(Utils.getValueFromSliderEvent(changeEvent));
    }
}
