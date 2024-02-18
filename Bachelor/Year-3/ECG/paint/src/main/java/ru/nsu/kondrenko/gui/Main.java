package ru.nsu.kondrenko.gui;

import ru.nsu.kondrenko.gui.controller.*;
import ru.nsu.kondrenko.gui.view.ToolsIconsSupplier;
import ru.nsu.kondrenko.gui.view.ToolsIconsSupplierImpl;
import ru.nsu.kondrenko.gui.view.View;
import ru.nsu.kondrenko.model.context.Context;
import ru.nsu.kondrenko.model.context.ContextAction;
import ru.nsu.kondrenko.model.context.ContextImpl;
import ru.nsu.kondrenko.model.image.ImageReaderImpl;
import ru.nsu.kondrenko.model.image.ImageSaverImpl;
import ru.nsu.kondrenko.model.image.ImageUtils;

import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.IOException;

public class Main {
    private static final String WINDOW_TITLE = "Simple Paint";
    private static final int MIN_FRAME_WIDTH = 640;
    private static final int MIN_FRAME_HEIGHT = 480;

    private static final BufferedImage START_IMAGE = ImageUtils.createBlankImage(
            MIN_FRAME_WIDTH,
            MIN_FRAME_HEIGHT
    );

    private static final Color START_COLOR = Color.BLACK;

    private static final int START_THICKNESS = 1;
    private static final int MIN_THICKNESS = 1;
    private static final int MAX_THICKNESS = 20;

    private static final int START_NUMBER_OF_SIDES = 4;
    private static final int MIN_NUMBER_OF_VERTICES = 3;
    private static final int MAX_NUMBER_OF_VERTICES = 16;

    private static final int START_RADIUS = 100;
    private static final int MIN_RADIUS = 1;
    private static final int MAX_RADIUS = 200;

    private static final int START_ROTATION = 0;
    private static final int MIN_ROTATION = 0;
    private static final int MAX_ROTATION = 359;

    public static void main(String[] args) {
        try {
            final Context context = new ContextImpl(
                    START_IMAGE,
                    START_COLOR,
                    START_THICKNESS,
                    START_RADIUS,
                    START_NUMBER_OF_SIDES,
                    START_ROTATION
            );

            final var imageReader = new ImageReaderImpl();
            final var imageSaver = new ImageSaverImpl();
            final var buttonsController = new ButtonsController(context);
            final var filesActionsController = new FilesActionsController(context, imageReader, imageSaver);
            final var mouseController = new MouseController(context);
            final var frameResizingController = new FrameResizingController(context);
            final var dialogWindowController = new DialogWindowController();

            final ToolsIconsSupplier toolsIconsSupplier = new ToolsIconsSupplierImpl();

            final View view = new View(
                    WINDOW_TITLE,
                    MIN_FRAME_WIDTH,
                    MIN_FRAME_HEIGHT,
                    MIN_THICKNESS,
                    MAX_THICKNESS,
                    MIN_NUMBER_OF_VERTICES,
                    MAX_NUMBER_OF_VERTICES,
                    MIN_RADIUS,
                    MAX_RADIUS,
                    MIN_ROTATION,
                    MAX_ROTATION,
                    imageReader.getSupportedFormats(),
                    buttonsController,
                    filesActionsController,
                    mouseController,
                    frameResizingController,
                    dialogWindowController,
                    toolsIconsSupplier
            );

            context.addListener(view);
            context.setAction(ContextAction.REPAINT);
            view.show();
        } catch (IOException exception) {
            System.err.printf("Cannot init view: %s%n", exception.getLocalizedMessage());
        }
    }
}
