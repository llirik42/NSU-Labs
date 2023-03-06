package ru.nsu.kondrenko.model;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

import java.util.HashMap;
import java.util.Map;

class UtilitiesTest {
    @Test
    void testReduceSpaces() {
        Map<String, String> tests = new HashMap<>() {{
            put("STRING1", "STRING1");
            put(" STRING2", "STRING2");
            put("STRING3 ", "STRING3");
            put(" STRING4 ", "STRING4");
            put("a b c", "a b c");
            put("x   y       z", "x y z");
            put("            p q       r  ", "p q r");
        }};

        for (var entry : tests.entrySet()) {
            assertEquals(entry.getValue(), Utilities.reduceSpaces(entry.getKey()));
        }
    }

    @Test
    void testIsLineFictive() {
        String[] fictiveLines = new String[]{
                "#",
                "# 12344",
                "    # 123312",
                "   # dsad dsa dsa $# dsa #",
                "                   ",
                "                  ",
                "\n\n\n"
        };

        for (var line : fictiveLines) {
            assertTrue(Utilities.isLineFictive(line, "#"));
        }
    }
}
