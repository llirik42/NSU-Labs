package ru.nsu.kondrenko.model.context;

import java.util.List;
import java.util.Map;

public interface Context {
    List<Double> getNumbersOnStack();

    Map<String, Double> getVariables();

    void pushValue(double value);

    void pushVariable(String name);

    double pop();

    double peek();

    boolean isVariableName(String string);

    void defineVariable(String name, double value);

    int getStackSize();

    void setExitFlag();

    boolean getExitFlag();
}
