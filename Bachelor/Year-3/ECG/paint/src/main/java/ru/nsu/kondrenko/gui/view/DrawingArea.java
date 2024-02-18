package ru.nsu.kondrenko.gui.view;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ComponentListener;
import java.awt.event.MouseListener;
import java.awt.image.BufferedImage;

public class DrawingArea extends JPanel {
    private BufferedImage image;

    public DrawingArea(MouseListener mouseListener, ComponentListener componentListener) {
        setBackground(Color.WHITE);
        addMouseListener(mouseListener);
        addComponentListener(componentListener);
    }

    void setImage(BufferedImage image) {
        this.image = image;
    }

    void resizeSoftly(int width, int height) {
        setPreferredSize(new Dimension(width, height));
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        g.drawImage(image, 0, 0, null);
    }
}
