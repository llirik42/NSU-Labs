package ru.nsu.kondrenko.model.instructionsfactory;

import ru.nsu.kondrenko.model.instructions.Instruction;
import ru.nsu.kondrenko.model.parsing.ParsedLine;

public interface InstructionsFactory {
    Instruction createInstruction(ParsedLine parsedLine);

    boolean isAdd(String instructionName);

    boolean isMultiply(String instructionName);

    boolean isSubtract(String instructionName);

    boolean isDivide(String instructionName);

    boolean isLog(String instructionName);
}
