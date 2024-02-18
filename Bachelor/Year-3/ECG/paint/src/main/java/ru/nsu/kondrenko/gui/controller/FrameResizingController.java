package ru.nsu.kondrenko.gui.controller;

import ru.nsu.kondrenko.model.context.Context;
import ru.nsu.kondrenko.model.image.ImageUtils;

import java.awt.*;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.image.BufferedImage;

public class FrameResizingController extends ComponentAdapter {
    private final Context context;

    public FrameResizingController(Context context) {
        this.context = context;
    }

    @Override
    public void componentResized(ComponentEvent componentEvent) {
        final Component drawingArea = componentEvent.getComponent();
        final int drawingAreaWidth = drawingArea.getWidth();
        final int drawingAreaHeight = drawingArea.getHeight();

        final BufferedImage oldImage = context.getImage();
        final int oldImageWidth = oldImage.getWidth();
        final int oldImageHeight = oldImage.getHeight();

        final int newImageWidth = Integer.max(drawingAreaWidth, oldImageWidth);
        final int newImageHeight = Integer.max(drawingAreaHeight, oldImageHeight);

        if (newImageWidth == oldImageWidth && newImageHeight == oldImageHeight) {
            return;
        }

        final BufferedImage newImage = ImageUtils.enhanceImage(oldImage, newImageWidth, newImageHeight);
        context.setImage(newImage);
    }
}
