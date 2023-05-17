package ru.nsu.kondrenko.model.instructionsfactory;

import ru.nsu.kondrenko.model.instructions.Instruction;
import ru.nsu.kondrenko.model.parsing.ParsedLine;

import java.util.HashMap;
import java.util.Map;

public class CachingInstructionsFactory implements InstructionsFactory {
    private final Map<String, Instruction> cachedInstructions = new HashMap<>();

    private final InstructionsFactory instructionsFactory;

    public CachingInstructionsFactory(InstructionsFactory instructionsFactory) {
        this.instructionsFactory = instructionsFactory;
    }

    @Override
    public Instruction createInstruction(ParsedLine parsedLine) {
        final String instructionName = parsedLine.getWords()[0];

        if (isLog(instructionName)) {
            return instructionsFactory.createInstruction(parsedLine);
        }

        if (cachedInstructions.containsKey(instructionName)) {
            return cachedInstructions.get(instructionName);
        }

        final Instruction newInstruction = instructionsFactory.createInstruction(parsedLine);
        cachedInstructions.put(instructionName, newInstruction);
        return newInstruction;
    }

    @Override
    public boolean isAdd(String instructionName) {
        return instructionsFactory.isAdd(instructionName);
    }

    @Override
    public boolean isMultiply(String instructionName) {
        return instructionsFactory.isMultiply(instructionName);
    }

    @Override
    public boolean isSubtract(String instructionName) {
        return instructionsFactory.isSubtract(instructionName);
    }

    @Override
    public boolean isDivide(String instructionName) {
        return instructionsFactory.isDivide(instructionName);
    }

    @Override
    public boolean isLog(String instructionName) {
        return instructionsFactory.isLog(instructionName);
    }
}
