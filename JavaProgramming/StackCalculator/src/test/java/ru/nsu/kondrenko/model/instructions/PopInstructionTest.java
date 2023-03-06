package ru.nsu.kondrenko.model.instructions;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

import ru.nsu.kondrenko.model.context.Context;
import ru.nsu.kondrenko.model.context.ContextExceptionsDecorator;
import ru.nsu.kondrenko.model.context.RAMContext;
import ru.nsu.kondrenko.model.exceptions.InstructionException;

class PopInstructionTest {
    @Test
    void testBasic() {
        Context context = new RAMContext();

        context.pushValue(2);
        context.pushValue(1);

        var instruction = new PopInstruction();

        instruction.execute(context, new String[]{});

        assertEquals(2, context.peek());
    }

    @Test
    void testEmptyStack() {
        Context context = new ContextExceptionsDecorator(new RAMContext());

        var instruction = new PopInstruction();

        assertThrows(InstructionException.class, () -> instruction.execute(context, new String[]{}));
    }
}
