package ru.nsu.kondrenko.model.instructions;

import ru.nsu.kondrenko.model.context.Context;

public interface Instruction {
    void execute(Context context, String[] operands);

    int getNeededElementsCountOnStack();

    int getNeededOperandsCount();
}
