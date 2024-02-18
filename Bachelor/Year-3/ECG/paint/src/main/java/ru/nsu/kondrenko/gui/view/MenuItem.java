package ru.nsu.kondrenko.gui.view;

public record MenuItem(String label, String actionCommand, MenuItemType type) {
    public MenuItem(String label, String actionCommand) {
        this(label, actionCommand, MenuItemType.DEFAULT_ITEM);
    }
}
