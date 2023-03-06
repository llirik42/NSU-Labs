package ru.nsu.kondrenko.model.instructions;

import ru.nsu.kondrenko.model.context.Context;

public class PushInstruction implements Instruction {
    @Override
    public void execute(Context context, String[] operands) {
        String valueToPush = operands[0];

        try {
            context.pushValue(Double.parseDouble(valueToPush));
        } catch (NumberFormatException exception) {
            context.pushVariable(valueToPush);
        }
    }

    @Override
    public int getNeededElementsCountOnStack() {
        return 0;
    }

    @Override
    public int getNeededOperandsCount() {
        return 1;
    }
}
