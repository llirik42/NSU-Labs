package ru.nsu.kondrenko.gui.view;

import javax.swing.*;
import java.awt.*;

abstract class FixedSizeFrame extends JFrame implements GameListener {
    protected FixedSizeFrame(Dimension dimension) {
        setVisible(true);
        setPreferredSize(dimension);
        setResizable(false);
    }
}
