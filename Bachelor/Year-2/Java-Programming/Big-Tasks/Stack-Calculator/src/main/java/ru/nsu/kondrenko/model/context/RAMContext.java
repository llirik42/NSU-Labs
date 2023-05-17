package ru.nsu.kondrenko.model.context;

import java.util.*;

public class RAMContext implements Context {
    private boolean exitFlag = false;

    private final Stack<Double> stack = new Stack<>();

    private final Map<String, Double> variables = new HashMap<>();

    @Override
    public List<Double> getNumbersOnStack() {
        List<Double> result = new ArrayList<>(stack);
        return Collections.unmodifiableList(result);
    }

    @Override
    public Map<String, Double> getVariables() {
        return Collections.unmodifiableMap(variables);
    }

    @Override
    public void pushValue(double value) {
        stack.push(value);
    }

    @Override
    public void pushVariable(String name) {
        pushValue(variables.get(name));
    }

    @Override
    public double pop() {
        return stack.pop();
    }

    @Override
    public double peek() {
        return stack.peek();
    }

    @Override
    public boolean isVariableName(String string) {
        return variables.containsKey(string);
    }

    @Override
    public void defineVariable(String name, double value) {
        variables.put(name, value);
    }

    @Override
    public int getStackSize() {
        return stack.size();
    }

    @Override
    public void setExitFlag() {
        exitFlag = true;
    }

    @Override
    public boolean getExitFlag() {
        return exitFlag;
    }
}
