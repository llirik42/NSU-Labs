package ru.nsu.kondrenko.gui.controller;

import ru.nsu.kondrenko.model.context.Context;
import ru.nsu.kondrenko.model.context.ContextAction;
import ru.nsu.kondrenko.model.image.ImageReader;
import ru.nsu.kondrenko.model.image.ImageSaver;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.Objects;

public class FilesActionsController implements ActionListener {
    private final Context context;
    private final ImageReader imageReader;
    private final ImageSaver imageSaver;

    public FilesActionsController(Context context, ImageReader imageReader, ImageSaver imageSaver) {
        this.context = context;
        this.imageReader = imageReader;
        this.imageSaver = imageSaver;
    }

    @Override
    public void actionPerformed(ActionEvent actionEvent) {
        handleJFileChooserEvent(actionEvent, (JFileChooser) actionEvent.getSource());
    }

    private void handleJFileChooserEvent(ActionEvent actionEvent, JFileChooser fileChooser) {
        final String actionCommand = actionEvent.getActionCommand();

        if (!Objects.equals(actionCommand, JFileChooser.APPROVE_SELECTION)) {
            return;
        }

        if (context.getAction() == ContextAction.SAVE_FILE) {
            handleApproveSavingFile(fileChooser);
        } else {
            handleApproveOpeningFile(fileChooser);
        }
    }

    private void handleApproveOpeningFile(JFileChooser fileChooser) {
        try {
            final BufferedImage image = imageReader.read(fileChooser.getSelectedFile());
            context.setImage(image);
            context.setAction(ContextAction.REPAINT);
        } catch (IOException exception) {
            context.pushError(exception.getLocalizedMessage());
        }
    }

    private void handleApproveSavingFile(JFileChooser fileChooser) {
        final String filterDescription = fileChooser.getFileFilter().getDescription();

        try {
            if (Objects.equals(filterDescription, "PNG")) {
                final File pngFile = new File(fileChooser.getSelectedFile().getAbsolutePath() + ".png");
                imageSaver.saveAsPNG(context.getImage(), pngFile);
            } else {
                throw new RuntimeException(String.format("Saving an image in an unsupported format \"%s\"", filterDescription));
            }
        } catch (IOException exception) {
            context.pushError(exception.getLocalizedMessage());
        }
    }
}
