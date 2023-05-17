package ru.nsu.kondrenko.view;

import javax.swing.*;

abstract class Frame extends JFrame {
    public abstract void setBodiesSuppliedCount(int newValue);

    public abstract void setEnginesSuppliedCount(int newValue);

    public abstract void setAccessoriesSuppliedCount(int newValue);

    public abstract void setCarsAssembledCount(int newValue);

    public abstract void setCarsSoldCount(int newValue);

    public abstract void setBodiesStoredCount(int newValue);

    public abstract void setEnginesStoredCount(int newValue);

    public abstract void setAccessoriesStoredCount(int newValue);

    public abstract void setCarsStoredCount(int newValue);

    public abstract void setTasksWaitingCount(int newValue);
}
