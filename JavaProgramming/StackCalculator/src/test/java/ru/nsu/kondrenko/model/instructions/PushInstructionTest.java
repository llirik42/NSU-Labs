package ru.nsu.kondrenko.model.instructions;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

import ru.nsu.kondrenko.model.context.Context;
import ru.nsu.kondrenko.model.context.ContextExceptionsDecorator;
import ru.nsu.kondrenko.model.context.RAMContext;
import ru.nsu.kondrenko.model.exceptions.InstructionException;

class PushInstructionTest {
    @Test
    void testNumber() {
        Context context = new RAMContext();

        Instruction instruction = new PushInstruction();

        instruction.execute(context, new String[]{"5"});

        assertEquals(5, context.peek());
    }

    @Test
    void testVariable() {
        Context context = new RAMContext();

        context.defineVariable("q", 42);

        Instruction instruction = new PushInstruction();

        instruction.execute(context, new String[]{"q"});

        assertEquals(42, context.peek());
    }

    @Test
    void testUnknownVariable() {
        Context context = new ContextExceptionsDecorator(new RAMContext());

        Instruction instruction = new PushInstruction();

        assertThrows(InstructionException.class, () -> instruction.execute(context, new String[]{"q"}));
    }
}
