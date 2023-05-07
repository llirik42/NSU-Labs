package ru.nsu.kondrenko.view;

import javax.swing.*;

class DefaultFrame extends Frame {
    private final JTable suppliedTable;
    private final JTable storedTable;

    public DefaultFrame(JTable suppliedTable, JTable storedTable) {
        this.suppliedTable = suppliedTable;
        this.storedTable = storedTable;
    }

    private static void setValueOfTable(JTable table, int column, int newValue) {
        table.setValueAt(String.valueOf(newValue), 0, column);
    }

    @Override
    public void setBodiesSuppliedCount(int newValue) {
        setValueOfTable(suppliedTable, 0, newValue);
    }

    @Override
    public void setEnginesSuppliedCount(int newValue) {
        setValueOfTable(suppliedTable, 1, newValue);
    }

    @Override
    public void setAccessoriesSuppliedCount(int newValue) {
        setValueOfTable(suppliedTable, 2, newValue);
    }

    @Override
    public void setCarsAssembledCount(int newValue) {
        setValueOfTable(suppliedTable, 3, newValue);
    }

    @Override
    public void setCarsSoldCount(int newValue) {
        setValueOfTable(suppliedTable, 4, newValue);
    }

    @Override
    public void setBodiesStoredCount(int newValue) {
        setValueOfTable(storedTable, 0, newValue);
    }

    @Override
    public void setEnginesStoredCount(int newValue) {
        setValueOfTable(storedTable, 1, newValue);
    }

    @Override
    public void setAccessoriesStoredCount(int newValue) {
        setValueOfTable(storedTable, 2, newValue);
    }

    @Override
    public void setCarsStoredCount(int newValue) {
        setValueOfTable(storedTable, 3, newValue);
    }

    @Override
    public void setTasksWaitingCount(int newValue) {
        setValueOfTable(storedTable, 4, newValue);
    }
}
