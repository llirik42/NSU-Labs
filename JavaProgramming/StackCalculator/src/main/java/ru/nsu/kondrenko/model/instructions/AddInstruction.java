package ru.nsu.kondrenko.model.instructions;

import ru.nsu.kondrenko.model.context.Context;

public class AddInstruction implements Instruction {
    @Override
    public void execute(Context context, String[] operands) {
        double operand1 = context.pop();
        double operand2 = context.pop();
        context.pushValue(operand1 + operand2);
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
