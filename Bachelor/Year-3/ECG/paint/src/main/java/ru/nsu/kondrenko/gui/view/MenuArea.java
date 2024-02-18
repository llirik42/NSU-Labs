package ru.nsu.kondrenko.gui.view;

import ru.nsu.kondrenko.gui.ActionCommands;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionListener;
import java.util.Arrays;
import java.util.List;
import java.util.function.Function;

public class MenuArea extends JPanel {
    private static final Font FONT = new Font("Go", Font.BOLD, 14);
    private static final Color MENU_BACKGROUND_COLOR = new Color(0.85f, 0.85f, 0.85f);
    private static final Color BUTTONS_FONT_COLOR = new Color(0.14f, 0.13f, 0.13f);
    private static final String FILE_MENU_TITLE = "File";
    private static final String EDIT_MENU_TITLE = "Edit";
    private static final String HELP_MENU_TITLE = "Info";

    private static final List<MenuItem> FILE_MENU_ITEMS = Arrays.asList(
            new MenuItem("Open", ActionCommands.OPEN),
            new MenuItem("Save", ActionCommands.SAVE),
            new MenuItem("Exit", ActionCommands.EXIT)
    );
    private static final List<MenuItem> EDIT_MENU_DRAWING_ITEMS = Arrays.asList(
            new MenuItem("Line", ActionCommands.DRAW_LINE, MenuItemType.DRAW_LINE_TOOL),
            new MenuItem("Polygon", ActionCommands.DRAW_POLYGON, MenuItemType.DRAW_POLYGON_TOOL),
            new MenuItem("Star", ActionCommands.DRAW_STAR, MenuItemType.DRAW_STAR_TOOL),
            new MenuItem("Fill", ActionCommands.FILL, MenuItemType.FILL_TOOL)
    );
    private static final List<MenuItem> EDIT_MENU_OTHER_ITEMS = Arrays.asList(
            new MenuItem("Clear", ActionCommands.CLEAR),
            new MenuItem("Select color", ActionCommands.SELECT_COLOR),
            new MenuItem("Select thickness", ActionCommands.SELECT_THICKNESS),
            new MenuItem("Select number of vertices", ActionCommands.SELECT_NUMBER_OF_VERTICES),
            new MenuItem("Select rotation", ActionCommands.SELECT_ROTATION),
            new MenuItem("Select radius", ActionCommands.SELECT_RADIUS)
    );
    private static final List<MenuItem> HELP_ABOUT_MENU_ITEMS = Arrays.asList(
            new MenuItem("Help", ActionCommands.SHOW_HELP),
            new MenuItem("About", ActionCommands.SHOW_ABOUT)
    );

    private final JMenuBar menuBar;
    private ButtonModel drawLineButtonModel;
    private ButtonModel drawPolygonButtonModel;
    private ButtonModel drawStarButtonModel;
    private ButtonModel fillButtonModel;
    private ButtonGroup drawButtonsGroup;

    public MenuArea(ActionListener actionListener) {
        setLayout(new FlowLayout(FlowLayout.LEFT));
        setBackground(MENU_BACKGROUND_COLOR);

        menuBar = new JMenuBar();
        menuBar.setBackground(MENU_BACKGROUND_COLOR);

        final List<Function<ActionListener, JMenu>> menusCreationMethods = Arrays.asList(
                this::createFileMenu,
                this::createEditMenu,
                this::createHelpMenu
        );

        for (final var it : menusCreationMethods) {
            final JMenu menu = it.apply(actionListener);
            menuBar.add(menu);
        }
    }

    private static JMenu createMenu(String label) {
        JMenu result = new JMenu(label);
        result.setBackground(MENU_BACKGROUND_COLOR);
        result.setForeground(BUTTONS_FONT_COLOR);
        result.setFont(FONT);
        return result;
    }

    private static JMenuItem createMenuItem(String label, String actionCommand, ActionListener actionListener) {
        final JMenuItem result = new JMenuItem(label);
        initButton(result, actionCommand, actionListener);
        return result;
    }

    private static JRadioButtonMenuItem createRadioButtonMenuItem(String label, String actionCommand, ActionListener actionListener) {
        final JRadioButtonMenuItem result = new JRadioButtonMenuItem(label);
        initButton(result, actionCommand, actionListener);
        return result;
    }

    private static void initButton(AbstractButton button, String actionCommand, ActionListener actionListener) {
        button.setBorderPainted(false);
        button.setFont(FONT);
        button.setForeground(BUTTONS_FONT_COLOR);
        button.addActionListener(actionListener);
        button.setActionCommand(actionCommand);
    }

    private static void pushItemToMenu(JMenu menu, List<MenuItem> items, ActionListener actionListener) {
        for (final var it : items) {
            final JMenuItem item = createMenuItem(it.label(), it.actionCommand(), actionListener);
            menu.add(item);
        }
    }

    public void setDrawLineSelected() {
        drawButtonsGroup.setSelected(drawLineButtonModel, true);
    }

    public void setDrawPolygonSelected() {
        drawButtonsGroup.setSelected(drawPolygonButtonModel, true);
    }

    public void setDrawStarSelected() {
        drawButtonsGroup.setSelected(drawStarButtonModel, true);
    }

    public void setFillSelected() {
        drawButtonsGroup.setSelected(fillButtonModel, true);
    }

    public JMenuBar getMenuBar() {
        return menuBar;
    }

    private JMenu createFileMenu(ActionListener actionListener) {
        final JMenu result = createMenu(FILE_MENU_TITLE);
        pushItemToMenu(result, FILE_MENU_ITEMS, actionListener);
        return result;
    }

    private JMenu createEditMenu(ActionListener actionListener) {
        final JMenu result = createMenu(EDIT_MENU_TITLE);

        drawButtonsGroup = new ButtonGroup();
        for (final var it : EDIT_MENU_DRAWING_ITEMS) {
            final JRadioButtonMenuItem item = createRadioButtonMenuItem(it.label(), it.actionCommand(), actionListener);
            drawButtonsGroup.add(item);
            result.add(item);
            final MenuItemType type = it.type();
            final ButtonModel buttonModel = item.getModel();
            switch (type) {
                case DRAW_LINE_TOOL -> drawLineButtonModel = buttonModel;
                case DRAW_POLYGON_TOOL -> drawPolygonButtonModel = buttonModel;
                case DRAW_STAR_TOOL -> drawStarButtonModel = buttonModel;
                case FILL_TOOL -> fillButtonModel = buttonModel;
            }
        }
        result.add(new JSeparator());
        pushItemToMenu(result, EDIT_MENU_OTHER_ITEMS, actionListener);

        return result;
    }

    private JMenu createHelpMenu(ActionListener actionListener) {
        final JMenu result = createMenu(HELP_MENU_TITLE);
        pushItemToMenu(result, HELP_ABOUT_MENU_ITEMS, actionListener);
        return result;
    }
}
