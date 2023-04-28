package ru.nsu.kondrenko.model.instructions;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;

import org.junit.jupiter.api.*;
import static org.junit.jupiter.api.Assertions.*;

import ru.nsu.kondrenko.model.context.RAMContext;
import ru.nsu.kondrenko.model.context.Context;

class PrintInstructionTest {
    private static final ByteArrayOutputStream outContent = new ByteArrayOutputStream();
    private static final PrintStream originalOut = System.out;

    @BeforeAll
    public static void setUpStream() {
        System.setOut(new PrintStream(outContent));
    }

    @AfterAll
    public static void restoreStream() {
        System.setOut(originalOut);
    }

    @AfterEach
    public void flushStream() {
        outContent.reset();
    }

    @Test
    void testBasic() {
        Context context = new RAMContext();

        context.pushValue(2);

        Instruction instruction = new PrintInstruction();

        instruction.execute(context, new String[]{});

        assertEquals("2.0\n", outContent.toString());
    }

    @Test
    void testNegative() {
        Context context = new RAMContext();

        context.pushValue(-2.5);

        Instruction instruction = new PrintInstruction();

        instruction.execute(context, new String[]{});

        assertEquals("-2.5\n", outContent.toString());
    }
}
