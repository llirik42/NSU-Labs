package ru.nsu.kondrenko.model.instructions;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

import ru.nsu.kondrenko.model.context.ContextExceptionsDecorator;
import ru.nsu.kondrenko.model.context.RAMContext;
import ru.nsu.kondrenko.model.context.Context;
import ru.nsu.kondrenko.model.exceptions.InstructionException;

class DefineInstructionTest {
    @Test
    void testSimpleDefine() {
        Context context = new RAMContext();

        Instruction instruction = new DefineInstruction();

        instruction.execute(context, new String[]{"q", "5"});

        assertTrue(context.isVariableName("q"));

        context.pushVariable("q");

        assertEquals(5, context.peek());
    }

    @Test
    void testDefineWithError() {
        Context context = new RAMContext();

        Instruction instruction = new DefineInstruction();

        assertThrows(InstructionException.class, () -> instruction.execute(context, new String[]{"q", "h"}));
    }

    @Test
    void testRedefine() {
        Context context = new ContextExceptionsDecorator(new RAMContext());

        context.defineVariable("q", 13);

        Instruction instruction = new DefineInstruction();

        assertThrows(InstructionException.class, () -> instruction.execute(context, new String[]{"q", "14"}));
    }
}
