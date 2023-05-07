package ru.nsu.kondrenko.view;

import ru.nsu.kondrenko.model.parsing.config.Config;
import ru.nsu.kondrenko.model.parsing.limits.Limits;

import javax.swing.event.ChangeListener;
import java.awt.event.WindowListener;

public interface AbstractFrameFactory {
    Frame createMainFrame(Config config,
                          Limits limits,
                          ChangeListener bodySuppliersCountListener,
                          ChangeListener engineSuppliersCountListener,
                          ChangeListener accessorySuppliersCountListener,
                          ChangeListener workersCountListener,
                          ChangeListener dealersCountListener,
                          ChangeListener bodySupplyTimeListener,
                          ChangeListener engineSupplyTimeListener,
                          ChangeListener accessorySupplyTimeListener,
                          ChangeListener carAssemblingTimeListener,
                          ChangeListener carRequestTimeListener,
                          WindowListener windowListener);
}
