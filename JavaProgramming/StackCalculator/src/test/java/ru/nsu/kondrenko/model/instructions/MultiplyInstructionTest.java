package ru.nsu.kondrenko.model.instructions;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

import ru.nsu.kondrenko.model.context.Context;
import ru.nsu.kondrenko.model.context.RAMContext;

class MultiplyInstructionTest {
    @Test
    void testZeroTimesZero() {
        Context context = new RAMContext();

        Instruction instruction = new AddInstruction();

        context.pushValue(0);
        context.pushValue(0);

        instruction.execute(context, new String[]{});
        assertEquals(0, context.peek());
    }

    @Test
    void testZeroTimesOne() {
        Context context = new RAMContext();

        Instruction instruction = new MultiplyInstruction();

        context.pushValue(0);
        context.pushValue(1);

        instruction.execute(context, new String[]{});
        assertEquals(0, context.peek());
    }

    @Test
    void testOneTimesOne() {
        Context context = new RAMContext();

        Instruction instruction = new MultiplyInstruction();

        context.pushValue(1);
        context.pushValue(1);

        instruction.execute(context, new String[]{});
        assertEquals(1, context.peek());
    }

    @Test
    void testFiveTimesThree() {
        Context context = new RAMContext();

        Instruction instruction = new MultiplyInstruction();

        context.pushValue(3);
        context.pushValue(5);

        instruction.execute(context, new String[]{});
        assertEquals(15, context.peek());
    }

    @Test
    void testNegative() {
        Context context = new RAMContext();

        Instruction instruction = new MultiplyInstruction();

        context.pushValue(-4);
        context.pushValue(-3);
        context.pushValue(2);
        context.pushValue(-1);
        context.pushValue(-1);
        context.pushValue(0);

        instruction.execute(context, new String[]{});
        assertEquals(-0.0, context.pop());

        instruction.execute(context, new String[]{});
        assertEquals(-2, context.pop());

        instruction.execute(context, new String[]{});
        assertEquals(12, context.pop());
    }
}
