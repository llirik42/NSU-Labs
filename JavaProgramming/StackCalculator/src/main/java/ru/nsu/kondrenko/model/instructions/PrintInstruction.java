package ru.nsu.kondrenko.model.instructions;

import ru.nsu.kondrenko.model.context.Context;

public class PrintInstruction implements Instruction {
    @Override
    public void execute(Context context, String[] operands) {
        System.out.println(context.peek());
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
