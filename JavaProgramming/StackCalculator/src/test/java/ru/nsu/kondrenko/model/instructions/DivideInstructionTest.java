package ru.nsu.kondrenko.model.instructions;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

import ru.nsu.kondrenko.model.context.RAMContext;
import ru.nsu.kondrenko.model.context.Context;
import ru.nsu.kondrenko.model.exceptions.InstructionException;

class DivideInstructionTest {
    @Test
    void testOneOne() {
        Context context = new RAMContext();

        context.pushValue(1);
        context.pushValue(1);

        Instruction instruction = new DivideInstruction();

        instruction.execute(context, new String[]{});

        assertEquals(1, context.peek());
    }

    @Test
    void testTwoFour() {
        Context context = new RAMContext();

        context.pushValue(4);
        context.pushValue(2);

        Instruction instruction = new DivideInstruction();

        instruction.execute(context, new String[]{});

        assertEquals(0.5, context.peek());
    }

    @Test
    void testZeroOne() {
        Context context = new RAMContext();

        context.pushValue(1);
        context.pushValue(0);

        Instruction instruction = new DivideInstruction();

        instruction.execute(context, new String[]{});

        assertEquals(0, context.peek());
    }

    @Test
    void testDivisionByZero() {
        Context context = new RAMContext();

        context.pushValue(0);
        context.pushValue(1);

        Instruction instruction = new DivideInstruction();

        assertThrows(InstructionException.class, () -> instruction.execute(context, new String[]{}));
    }
}
