package ru.nsu.kondrenko.model.instructions;

import ru.nsu.kondrenko.model.context.Context;

public class PopInstruction implements Instruction {
    @Override
    public void execute(Context context, String[] operands) {
        context.pop();
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
