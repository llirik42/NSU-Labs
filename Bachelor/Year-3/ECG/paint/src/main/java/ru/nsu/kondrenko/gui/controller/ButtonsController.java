package ru.nsu.kondrenko.gui.controller;

import ru.nsu.kondrenko.gui.ActionCommands;
import ru.nsu.kondrenko.model.context.Context;
import ru.nsu.kondrenko.model.context.ContextAction;
import ru.nsu.kondrenko.model.context.ContextTools;
import ru.nsu.kondrenko.model.image.ImageUtils;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.util.HashMap;
import java.util.Map;

public class ButtonsController implements ActionListener {
    private final Map<String, Runnable> actionCommandsMap = new HashMap<>();
    private final Context context;

    public ButtonsController(Context context) {
        this.context = context;

        actionCommandsMap.put(ActionCommands.OPEN, this::handleOpenActionCommand);
        actionCommandsMap.put(ActionCommands.SAVE, this::handleSaveActionCommand);
        actionCommandsMap.put(ActionCommands.EXIT, this::handleExitActionCommand);
        actionCommandsMap.put(ActionCommands.DRAW_LINE, this::handleDrawLineActionCommand);
        actionCommandsMap.put(ActionCommands.DRAW_POLYGON, this::handleDrawPolygonActionCommand);
        actionCommandsMap.put(ActionCommands.DRAW_STAR, this::handleDrawStarActionCommand);
        actionCommandsMap.put(ActionCommands.FILL, this::handleFillActionCommand);
        actionCommandsMap.put(ActionCommands.CLEAR, this::handleClearActionCommand);
        actionCommandsMap.put(ActionCommands.SELECT_THICKNESS, this::handleSelectThicknessActionCommand);
        actionCommandsMap.put(ActionCommands.SELECT_NUMBER_OF_VERTICES, this::handleSelectNumberOfVerticesActionCommand);
        actionCommandsMap.put(ActionCommands.SELECT_ROTATION, this::handleSelectRotationActionCommand);
        actionCommandsMap.put(ActionCommands.SELECT_RADIUS, this::handleSelectRadiusActionCommand);
        actionCommandsMap.put(ActionCommands.SELECT_COLOR, this::handleSelectColorActionCommand);
        actionCommandsMap.put(ActionCommands.CHANGE_COLOR_TO_BLACK, this::handleChangeColorToBlackActionCommand);
        actionCommandsMap.put(ActionCommands.CHANGE_COLOR_TO_WHITE, this::handleChangeColorToWhiteActionCommand);
        actionCommandsMap.put(ActionCommands.CHANGE_COLOR_TO_RED, this::handleChangeColorToRedActionCommand);
        actionCommandsMap.put(ActionCommands.CHANGE_COLOR_TO_GREEN, this::handleChangeColorToGreenActionCommand);
        actionCommandsMap.put(ActionCommands.CHANGE_COLOR_TO_BLUE, this::handleChangeColorToBlueActionCommand);
        actionCommandsMap.put(ActionCommands.CHANGE_COLOR_TO_MAGENTA, this::handleChangeColorToMagentaActionCommand);
        actionCommandsMap.put(ActionCommands.CHANGE_COLOR_TO_PINK, this::handleChangeColorToPinkActionCommand);
        actionCommandsMap.put(ActionCommands.CHANGE_COLOR_TO_ORANGE, this::handleChangeColorToOrangeActionCommand);
        actionCommandsMap.put(ActionCommands.CHANGE_COLOR_TO_YELLOW, this::handleChangeColorToYellowActionCommand);
        actionCommandsMap.put(ActionCommands.CHANGE_COLOR_TO_CYAN, this::handleChangeColorToCyanActionCommand);
        actionCommandsMap.put(ActionCommands.SHOW_HELP, this::handleShowHelpActionCommand);
        actionCommandsMap.put(ActionCommands.SHOW_ABOUT, this::handleShowAboutActionCommand);
    }

    @Override
    public void actionPerformed(ActionEvent actionEvent) {
        actionCommandsMap.get(actionEvent.getActionCommand()).run();
    }

    private void handleOpenActionCommand() {
        context.setAction(ContextAction.OPEN_FILE);
    }

    private void handleSaveActionCommand() {
        context.setAction(ContextAction.SAVE_FILE);
    }

    private void handleExitActionCommand() {
        context.setAction(ContextAction.EXIT);
    }

    private void handleDrawLineActionCommand() {
        context.setTool(ContextTools.DRAW_LINE);
    }

    private void handleDrawPolygonActionCommand() {
        context.setTool(ContextTools.DRAW_POLYGON);
    }

    private void handleDrawStarActionCommand() {
        context.setTool(ContextTools.DRAW_STAR);
    }

    private void handleFillActionCommand() {
        context.setTool(ContextTools.FILL);
    }

    private void handleClearActionCommand() {
        final BufferedImage currentImage = context.getImage();
        final int width = currentImage.getWidth();
        final int height = currentImage.getHeight();
        context.setImage(ImageUtils.createBlankImage(width, height));
        context.setAction(ContextAction.REPAINT);
    }

    private void handleSelectThicknessActionCommand() {
        context.setAction(ContextAction.SELECT_THICKNESS);
    }

    private void handleSelectNumberOfVerticesActionCommand() {
        context.setAction(ContextAction.SELECT_NUMBER_OF_VERTICES);
    }

    private void handleSelectRotationActionCommand() {
        context.setAction(ContextAction.SELECT_ROTATION);
    }

    private void handleSelectRadiusActionCommand() {
        context.setAction(ContextAction.SELECT_RADIUS);
    }

    private void handleSelectColorActionCommand() {
        context.setAction(ContextAction.SELECT_COLOR);
    }

    private void handleChangeColorToBlackActionCommand() {
        context.setColor(Color.BLACK);
    }

    private void handleChangeColorToWhiteActionCommand() {
        context.setColor(Color.WHITE);
    }

    private void handleChangeColorToRedActionCommand() {
        context.setColor(Color.RED);
    }

    private void handleChangeColorToGreenActionCommand() {
        context.setColor(Color.GREEN);
    }

    private void handleChangeColorToBlueActionCommand() {
        context.setColor(Color.BLUE);
    }

    private void handleChangeColorToMagentaActionCommand() {
        context.setColor(Color.MAGENTA);
    }

    private void handleChangeColorToPinkActionCommand() {
        context.setColor(Color.PINK);
    }

    private void handleChangeColorToOrangeActionCommand() {
        context.setColor(Color.ORANGE);
    }

    private void handleChangeColorToYellowActionCommand() {
        context.setColor(Color.YELLOW);
    }

    private void handleChangeColorToCyanActionCommand() {
        context.setColor(Color.CYAN);
    }

    private void handleShowHelpActionCommand() {
        context.setAction(ContextAction.DISPLAY_HELP);
    }

    private void handleShowAboutActionCommand() {
        context.setAction(ContextAction.DISPLAY_ABOUT);
    }
}
