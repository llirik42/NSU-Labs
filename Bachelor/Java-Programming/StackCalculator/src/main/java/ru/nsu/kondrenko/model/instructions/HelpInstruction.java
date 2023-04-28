package ru.nsu.kondrenko.model.instructions;

import ru.nsu.kondrenko.model.Utilities;
import ru.nsu.kondrenko.model.context.Context;

public class HelpInstruction implements Instruction {
    @Override
    public void execute(Context context, String[] operands) {
        System.out.println(Utilities.getHelp());
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
