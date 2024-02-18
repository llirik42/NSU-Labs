package ru.nsu.kondrenko.gui.view;

import ru.nsu.kondrenko.gui.ActionCommands;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionListener;
import java.util.Arrays;
import java.util.List;

public class ToolsArea extends JPanel {
    private static final Color AREA_BACKGROUND_COLOR = new Color(0.85f, 0.85f, 0.85f);
    private static final Color BUTTONS_BACKGROUND_COLOR = new Color(0.72f, 0.72f, 0.71f);
    private static final int TOOL_SIZE = 26;
    private static final int ICON_SIZE = 20;

    private JToggleButton drawLineToggleButton;
    private JToggleButton drawPolygonToggleButton;
    private JToggleButton drawStarToggleButton;
    private JToggleButton fillToggleButton;

    public ToolsArea(ToolsIconsSupplier toolsIconsSupplier, ActionListener actionListener) {
        final List<ToolButton> toolButtonsProperties = Arrays.asList(
                new ToolButton(toolsIconsSupplier.getDrawLineIcon(), ActionCommands.DRAW_LINE, "draw line"),
                new ToolButton(toolsIconsSupplier.getDrawPolygonIcon(), ActionCommands.DRAW_POLYGON, "draw polygon"),
                new ToolButton(toolsIconsSupplier.getDrawStarIcon(), ActionCommands.DRAW_STAR, "draw star"),
                new ToolButton(toolsIconsSupplier.getFillIcon(), ActionCommands.FILL, "fill"),
                new ToolButton(toolsIconsSupplier.getClearIcon(), ActionCommands.CLEAR, "clear"),
                new ToolButton(toolsIconsSupplier.getSelectThicknessIcon(), ActionCommands.SELECT_THICKNESS, "select thickness"),
                new ToolButton(toolsIconsSupplier.getSelectNumberOfVerticesIcon(), ActionCommands.SELECT_NUMBER_OF_VERTICES, "select number of vertices"),
                new ToolButton(toolsIconsSupplier.getSelectRadiusIcon(), ActionCommands.SELECT_RADIUS, "select radius"),
                new ToolButton(toolsIconsSupplier.getSelectRotationIcon(), ActionCommands.SELECT_ROTATION, "select rotation"),
                new ToolButton(toolsIconsSupplier.getSelectColorIcon(), ActionCommands.SELECT_COLOR, "select color"),
                new ToolButton(toolsIconsSupplier.getBlackIcon(), ActionCommands.CHANGE_COLOR_TO_BLACK, "change color to black"),
                new ToolButton(toolsIconsSupplier.getWhiteIcon(), ActionCommands.CHANGE_COLOR_TO_WHITE, "change color to white"),
                new ToolButton(toolsIconsSupplier.getRedIcon(), ActionCommands.CHANGE_COLOR_TO_RED, "change color to red"),
                new ToolButton(toolsIconsSupplier.getGreenIcon(), ActionCommands.CHANGE_COLOR_TO_GREEN, "change color to green"),
                new ToolButton(toolsIconsSupplier.getBlueIcon(), ActionCommands.CHANGE_COLOR_TO_BLUE, "change color to blue"),
                new ToolButton(toolsIconsSupplier.getMagentaIcon(), ActionCommands.CHANGE_COLOR_TO_MAGENTA, "change color to magenta"),
                new ToolButton(toolsIconsSupplier.getPinkIcon(), ActionCommands.CHANGE_COLOR_TO_PINK, "change color to pink"),
                new ToolButton(toolsIconsSupplier.getOrangeIcon(), ActionCommands.CHANGE_COLOR_TO_ORANGE, "change color to orange"),
                new ToolButton(toolsIconsSupplier.getYellowIcon(), ActionCommands.CHANGE_COLOR_TO_YELLOW, "change color to yellow"),
                new ToolButton(toolsIconsSupplier.getCyanIcon(), ActionCommands.CHANGE_COLOR_TO_CYAN, "change color to cyan")
        );

        setLayout(new FlowLayout(FlowLayout.LEFT));
        setBackground(AREA_BACKGROUND_COLOR);

        for (final var it : toolButtonsProperties) {
            final String actionCommand = it.actionCommand();

            AbstractButton button;

            switch (actionCommand) {
                case ActionCommands.DRAW_LINE -> {
                    drawLineToggleButton = createToolToggleButton(it.icon(), actionCommand, it.tip(), actionListener);
                    button = drawLineToggleButton;
                }

                case ActionCommands.DRAW_POLYGON -> {
                    drawPolygonToggleButton = createToolToggleButton(it.icon(), actionCommand, it.tip(), actionListener);
                    button = drawPolygonToggleButton;
                }

                case ActionCommands.DRAW_STAR -> {
                    drawStarToggleButton = createToolToggleButton(it.icon(), actionCommand, it.tip(), actionListener);
                    button = drawStarToggleButton;
                }

                case ActionCommands.FILL -> {
                    fillToggleButton = createToolToggleButton(it.icon(), actionCommand, it.tip(), actionListener);
                    button = fillToggleButton;
                }

                default -> button = createToolButton(it.icon(), actionCommand, it.tip(), actionListener);
            }

            add(button);
        }
    }

    private static ImageIcon scaleIcon(ImageIcon icon) {
        final Image image = icon.getImage();
        final Image scaledImage = image.getScaledInstance(ICON_SIZE, ICON_SIZE, Image.SCALE_SMOOTH);
        return new ImageIcon(scaledImage);
    }

    private static void initButton(AbstractButton button, String actionCommand, String tip, ActionListener actionListener) {
        button.setFocusPainted(false);
        button.setActionCommand(actionCommand);
        button.setToolTipText(tip);
        button.addActionListener(actionListener);
        button.setPreferredSize(new Dimension(TOOL_SIZE, TOOL_SIZE));
        button.setBackground(BUTTONS_BACKGROUND_COLOR);
        button.setBorderPainted(false);
    }

    public void setDrawLineSelected() {
        drawLineToggleButton.setSelected(true);
    }

    public void setDrawPolygonSelected() {
        drawPolygonToggleButton.setSelected(true);
    }

    public void setDrawStarSelected() {
        drawStarToggleButton.setSelected(true);
    }

    public void setFillSelected() {
        fillToggleButton.setSelected(true);
    }

    public void unselectAll() {
        drawLineToggleButton.setSelected(false);
        drawPolygonToggleButton.setSelected(false);
        drawStarToggleButton.setSelected(false);
        fillToggleButton.setSelected(false);
    }

    private JToggleButton createToolToggleButton(ImageIcon icon, String actionCommand, String tip, ActionListener actionListener) {
        final JToggleButton result = new JToggleButton(scaleIcon(icon));
        initButton(result, actionCommand, tip, actionListener);
        return result;
    }

    private JButton createToolButton(ImageIcon icon, String actionCommand, String tip, ActionListener actionListener) {
        final JButton result = new JButton(scaleIcon(icon));
        initButton(result, actionCommand, tip, actionListener);
        return result;
    }
}
