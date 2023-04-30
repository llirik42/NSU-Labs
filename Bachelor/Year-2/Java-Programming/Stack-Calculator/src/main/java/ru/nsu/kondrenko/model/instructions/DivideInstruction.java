package ru.nsu.kondrenko.model.instructions;

import ru.nsu.kondrenko.model.context.Context;
import ru.nsu.kondrenko.model.exceptions.DivisionByZeroException;
import ru.nsu.kondrenko.model.exceptions.InstructionException;

public class DivideInstruction implements Instruction {
    @Override
    public void execute(Context context, String[] operands) {
        double operand1 = context.pop();
        double operand2 = context.pop();

        if (operand2 == 0) {
            throw new DivisionByZeroException();
        }

        context.pushValue(operand1 / operand2);
    }

    @Override
    public int getNeededElementsCountOnStack() {
        return 2;
    }

    @Override
    public int getNeededOperandsCount() {
        return 0;
    }
}
