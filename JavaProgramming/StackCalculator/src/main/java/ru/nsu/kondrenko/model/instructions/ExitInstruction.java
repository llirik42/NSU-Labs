package ru.nsu.kondrenko.model.instructions;

import ru.nsu.kondrenko.model.context.Context;

public class ExitInstruction implements Instruction {
    @Override
    public void execute(Context context, String[] operands) {
        context.setExitFlag();
    }

    @Override
    public int getNeededElementsCountOnStack() {
        return 0;
    }

    @Override
    public int getNeededOperandsCount() {
        return 0;
    }
}
