package ru.nsu.kondrenko.model.context;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

class RAMContextTest {
    @Test
    void testPushValuePopV1() {
        Context context = new RAMContext();

        context.pushValue(1);
        assertEquals(1, context.pop());

        context.pushValue(3);
        assertEquals(3, context.pop());

        context.pushValue(4.675);
        assertEquals(4.675, context.pop());

        context.pushValue(-4.13);
        assertEquals(-4.13, context.pop());

        context.pushValue(-0.5);
        assertEquals(-0.5, context.pop());

        context.pushValue(3.61);
        assertEquals(3.61, context.pop());
    }

    @Test
    void testPushValuePopV2() {
        Context context = new RAMContext();

        context.pushValue(1);
        context.pushValue(3);
        context.pushValue(4.675);
        context.pushValue(-4.13);
        context.pushValue(-0.5);
        context.pushValue(3.61);

        assertEquals(3.61, context.pop());
        assertEquals(-0.5, context.pop());
        assertEquals(-4.13, context.pop());
        assertEquals(4.675, context.pop());
        assertEquals(3, context.pop());
        assertEquals(1, context.pop());
    }

    @Test
    void testPushPeek() {
        Context context = new RAMContext();

        context.pushValue(1);
        context.pushValue(3);

        assertEquals(3, context.peek());
        assertEquals(3, context.peek());
        assertEquals(3, context.peek());
    }

    @Test
    void testIsVariableName() {
        Context context = new RAMContext();

        context.defineVariable("a", 5);
        context.defineVariable("b", 0);
        context.defineVariable("Hello", -3);
        context.defineVariable("a5", -42);

        assertTrue(context.isVariableName("a"));
        assertTrue(context.isVariableName("b"));
        assertTrue(context.isVariableName("Hello"));
        assertTrue(context.isVariableName("a5"));

        assertFalse(context.isVariableName("A"));
        assertFalse(context.isVariableName("B"));
        assertFalse(context.isVariableName("A5"));
        assertFalse(context.isVariableName("q"));
        assertFalse(context.isVariableName("w"));
    }

    @Test
    void testDefinePushVariable() {
        Context context = new RAMContext();

        context.defineVariable("a", 5);
        context.defineVariable("b", 0);
        context.defineVariable("Hello", -3);
        context.defineVariable("a5", -42);

        context.pushVariable("a");
        context.pushVariable("b");
        context.pushVariable("Hello");
        context.pushVariable("a5");

        assertEquals(-42, context.pop());
        assertEquals(-3, context.pop());
        assertEquals(0, context.pop());
        assertEquals(5, context.pop());
    }
}
