package ru.nsu.kondrenko.model.instructions;

import ru.nsu.kondrenko.model.context.Context;
import ru.nsu.kondrenko.model.exceptions.LogDecoratorException;

import java.io.*;
import java.util.List;
import java.util.Map;

public class LoggingDecorator implements Instruction {
    private static final String LOG_FILE_NAME = "decorator.log";

    private final Instruction instruction;

    public LoggingDecorator(Instruction instruction) {
        this.instruction = instruction;
    }

    @Override
    public void execute(Context context, String[] operands) {
        writeContextState(context, operands, true);

        String[] tmp = new String[operands.length - 1];
        System.arraycopy(operands, 1, tmp, 0, operands.length - 1);
        instruction.execute(context, tmp);

        writeContextState(context, operands, false);
    }

    @Override
    public int getNeededElementsCountOnStack() {
        return instruction.getNeededElementsCountOnStack();
    }

    @Override
    public int getNeededOperandsCount() {
        return instruction.getNeededOperandsCount() + 1; // +1 for the name of instruction to decorate
    }

    private void writeContextState(Context context, String[] operands, boolean before) {
        try (FileWriter fileWriter = new FileWriter(LOG_FILE_NAME, true);
             BufferedWriter bufferedWriter = new BufferedWriter(fileWriter);
             PrintWriter out = new PrintWriter(bufferedWriter)) {

            final List<Double> elementsOnStack = context.getNumbersOnStack();
            final Map<String, Double> variables = context.getVariables();

            for (int i = 1; i < operands.length; i++) {
                out.write(operands[i] + " ");
            }
            out.write("\n");

            if (before) {
                out.write("BEFORE\n");
            } else {
                out.write("AFTER\n");
            }

            out.write("Stack: ");
            for (var it : elementsOnStack) {
                out.write(it + " ");
            }
            out.write("\n");

            out.write("Variables: ");
            for (var it : variables.entrySet()) {
                out.write(String.format("%s=%f ", it.getKey(), it.getValue()));
            }
            out.write("\n\n");
        } catch (IOException exception) {
            throw new LogDecoratorException(exception);
        }
    }
}
