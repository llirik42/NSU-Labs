package ru.nsu.kondrenko.model.context;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

import ru.nsu.kondrenko.model.exceptions.InstructionException;

class ContextExceptionsDecoratorTest {

    @Test
    void testPushingDefinedVariable() {
        Context context = new ContextExceptionsDecorator(new RAMContext());

        context.defineVariable("q", 5);

        assertDoesNotThrow(() -> context.pushVariable("q"));
    }
    @Test
    void testPushingUndefinedVariable() {
        Context context = new ContextExceptionsDecorator(new RAMContext());

        assertThrows(InstructionException.class, () -> context.pushVariable("q"));
    }

    @Test
    void testRedefiningVariable() {
        Context context = new ContextExceptionsDecorator(new RAMContext());

        context.defineVariable("q", 42);

        assertDoesNotThrow(() -> context.defineVariable("a", 5));
        assertThrows(InstructionException.class, () -> context.defineVariable("q", 42));
        assertThrows(InstructionException.class, () -> context.defineVariable("q", 45));
    }

    @Test
    void testEmptyStack() {
        Context context = new ContextExceptionsDecorator(new RAMContext());

        assertThrows(InstructionException.class, context::peek);
        assertThrows(InstructionException.class, context::pop);
    }
}
