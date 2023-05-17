package ru.nsu.kondrenko.model.context;

import java.util.EmptyStackException;
import java.util.List;
import java.util.Map;

import ru.nsu.kondrenko.model.exceptions.InstructionException;

public class ContextExceptionsDecorator implements Context {
    private final Context context;

    public ContextExceptionsDecorator(Context context) {
        this.context = context;
    }

    @Override
    public List<Double> getNumbersOnStack() {
        return context.getNumbersOnStack();
    }

    @Override
    public Map<String, Double> getVariables() {
        return context.getVariables();
    }

    @Override
    public void pushValue(double value) {
        context.pushValue(value);
    }

    @Override
    public void pushVariable(String name) {
        if (!isVariableName(name)) {
            throw new InstructionException("Using of undefined variable " + name);
        }

        context.pushVariable(name);
    }

    @Override
    public double pop() {
        try {
            return context.pop();
        } catch (EmptyStackException exception) {
            throw new InstructionException("Not enough operands on the stack", exception);
        }
    }

    @Override
    public double peek() {
        try {
            return context.peek();
        } catch (EmptyStackException exception) {
            throw new InstructionException("Not enough operands on the stack", exception);
        }
    }

    @Override
    public boolean isVariableName(String string) {
        return context.isVariableName(string);
    }

    @Override
    public void defineVariable(String name, double value) {
        if (context.isVariableName(name)) {
            throw new InstructionException("Redefining of variable " + name);
        }

        context.defineVariable(name, value);
    }

    @Override
    public int getStackSize() {
        return context.getStackSize();
    }

    @Override
    public void setExitFlag() {
        context.setExitFlag();
    }

    @Override
    public boolean getExitFlag() {
        return context.getExitFlag();
    }
}
