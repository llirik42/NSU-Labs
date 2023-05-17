package ru.nsu.kondrenko.model.instructions;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

import ru.nsu.kondrenko.model.context.RAMContext;
import ru.nsu.kondrenko.model.context.Context;

class AddInstructionTest {
    @Test
    void testZeroPlusZero() {
        Context context = new RAMContext();

        Instruction instruction = new AddInstruction();

        context.pushValue(0);
        context.pushValue(0);

        instruction.execute(context, new String[]{});
        assertEquals(0, context.peek());
    }

    @Test
    void testZeroPlusOne() {
        Context context = new RAMContext();

        Instruction instruction = new AddInstruction();

        context.pushValue(0);
        context.pushValue(1);

        instruction.execute(context, new String[]{});
        assertEquals(1, context.peek());
    }

    @Test
    void testOnePlusMinusOne() {
        Context context = new RAMContext();

        Instruction instruction = new AddInstruction();

        context.pushValue(-1);
        context.pushValue(1);

        instruction.execute(context, new String[]{});
        assertEquals(0, context.peek());
    }

    @Test
    void testFivePlusSix() {
        Context context = new RAMContext();

        Instruction instruction = new AddInstruction();

        context.pushValue(6);
        context.pushValue(5);

        instruction.execute(context, new String[]{});
        assertEquals(11, context.peek());
    }

    @Test
    void testMinusFivePlusSix() {
        Context context = new RAMContext();

        Instruction instruction = new AddInstruction();

        context.pushValue(6);
        context.pushValue(-5);

        instruction.execute(context, new String[]{});
        assertEquals(1, context.peek());
    }

    @Test
    void testFivePlusMinusSix() {
        Context context = new RAMContext();

        Instruction instruction = new AddInstruction();

        context.pushValue(-6);
        context.pushValue(5);

        instruction.execute(context, new String[]{});
        assertEquals(-1, context.peek());
    }
}
