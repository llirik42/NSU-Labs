package ru.nsu.kondrenko.model.instructions;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

import ru.nsu.kondrenko.model.context.Context;
import ru.nsu.kondrenko.model.context.RAMContext;

class SubtractInstructionTest {
    @Test
    void testZeroMinusZero() {
        Context context = new RAMContext();

        Instruction instruction = new SubtractInstruction();

        context.pushValue(0);
        context.pushValue(0);

        instruction.execute(context, new String[]{});
        assertEquals(0, context.peek());
    }

    @Test
    void testOneMinusZero() {
        Context context = new RAMContext();

        Instruction instruction = new SubtractInstruction();

        context.pushValue(0);
        context.pushValue(1);

        instruction.execute(context, new String[]{});
        assertEquals(1, context.peek());
    }

    @Test
    void testZeroMinusOne() {
        Context context = new RAMContext();

        Instruction instruction = new SubtractInstruction();

        context.pushValue(1);
        context.pushValue(0);

        instruction.execute(context, new String[]{});
        assertEquals(-1, context.peek());
    }

    @Test
    void testOneMinusOne() {
        Context context = new RAMContext();

        Instruction instruction = new SubtractInstruction();

        context.pushValue(1);
        context.pushValue(1);

        instruction.execute(context, new String[]{});
        assertEquals(0, context.peek());
    }

    @Test
    void testFiveMinusSix() {
        Context context = new RAMContext();

        Instruction instruction = new AddInstruction();

        context.pushValue(-6);
        context.pushValue(5);

        instruction.execute(context, new String[]{});
        assertEquals(-1, context.peek());
    }
}