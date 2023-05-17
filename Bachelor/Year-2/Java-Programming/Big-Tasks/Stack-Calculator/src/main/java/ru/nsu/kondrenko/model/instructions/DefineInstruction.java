package ru.nsu.kondrenko.model.instructions;

import ru.nsu.kondrenko.model.context.Context;
import ru.nsu.kondrenko.model.exceptions.InstructionException;

public class DefineInstruction implements Instruction {
    @Override
    public void execute(Context context, String[] operands) {
        try {
            String nameOfVariable = operands[1];
            double valueOfVariable = Double.parseDouble(operands[2]);
            context.defineVariable(nameOfVariable, valueOfVariable);
        } catch (NumberFormatException exception) {
            throw new InstructionException("Incorrect value of a new variable", exception);
        }
    }

    @Override
    public int getNeededElementsCountOnStack() {
        return 0;
    }

    @Override
    public int getNeededOperandsCount() {
        return 2;
    }
}
