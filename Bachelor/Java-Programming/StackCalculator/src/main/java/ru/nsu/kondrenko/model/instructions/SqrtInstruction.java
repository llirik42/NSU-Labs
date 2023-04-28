package ru.nsu.kondrenko.model.instructions;

import ru.nsu.kondrenko.model.context.Context;
import ru.nsu.kondrenko.model.exceptions.InstructionException;

public class SqrtInstruction implements Instruction {
    @Override
    public void execute(Context context, String[] operands) {
        double operand = context.pop();

        if (operand < 0) {
            throw new InstructionException("sqrt(a) where a < 0");
        }

        context.pushValue(Math.sqrt(operand));
    }

    @Override
    public int getNeededElementsCountOnStack() {
        return 1;
    }

    @Override
    public int getNeededOperandsCount() {
        return 0;
    }
}
