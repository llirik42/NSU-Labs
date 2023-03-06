package ru.nsu.kondrenko.model.instructions;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

import ru.nsu.kondrenko.model.context.Context;
import ru.nsu.kondrenko.model.context.ContextExceptionsDecorator;
import ru.nsu.kondrenko.model.context.RAMContext;
import ru.nsu.kondrenko.model.exceptions.InstructionException;

class SqrtInstructionTest {
    @Test
    void testZero() {
        Context context = new RAMContext();

        context.pushValue(0);

        Instruction instruction = new SqrtInstruction();

        instruction.execute(context, new String[]{});

        assertEquals(0, context.peek());
    }

    @Test
    void testOne() {
        Context context = new RAMContext();

        context.pushValue(0);

        Instruction instruction = new SqrtInstruction();

        instruction.execute(context, new String[]{});

        assertEquals(0, context.peek());
    }

    @Test
    void testFour() {
        Context context = new RAMContext();

        context.pushValue(4);

        Instruction instruction = new SqrtInstruction();

        instruction.execute(context, new String[]{});

        assertEquals(2, context.peek());
    }

    @Test
    void testTwentyFive() {
        Context context = new RAMContext();

        context.pushValue(25);

        Instruction instruction = new SqrtInstruction();

        instruction.execute(context, new String[]{});

        assertEquals(5, context.peek());
    }

    @Test
    void testNegative() {
        Context context = new ContextExceptionsDecorator(new RAMContext());

        context.pushValue(-1);

        Instruction instruction = new SqrtInstruction();

        assertThrows(InstructionException.class, () -> instruction.execute(context, new String[]{}));
    }
}
