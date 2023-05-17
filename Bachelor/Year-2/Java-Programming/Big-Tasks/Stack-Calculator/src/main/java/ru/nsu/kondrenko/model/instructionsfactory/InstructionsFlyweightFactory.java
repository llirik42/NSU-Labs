package ru.nsu.kondrenko.model.instructionsfactory;

import ru.nsu.kondrenko.model.instructions.Flyweight;
import ru.nsu.kondrenko.model.instructions.FlyweightOperation;
import ru.nsu.kondrenko.model.instructions.Instruction;
import ru.nsu.kondrenko.model.parsing.ParsedLine;

public class InstructionsFlyweightFactory implements InstructionsFactory {
    private final Flyweight flyweight = new Flyweight();

    private final InstructionsFactory instructionsFactory;

    public InstructionsFlyweightFactory(InstructionsFactory instructionsFactory) {
        this.instructionsFactory = instructionsFactory;
    }

    @Override
    public Instruction createInstruction(ParsedLine parsedLine) {
        final String instructionName = parsedLine.getWords()[0];

        FlyweightOperation operation = null;

        if (isAdd(instructionName)) {
            operation = FlyweightOperation.ADD;
        }
        if (isMultiply(instructionName)) {
            operation = FlyweightOperation.MULTIPLY;
        }
        if (isSubtract(instructionName)) {
            operation = FlyweightOperation.SUBTRACT;
        }
        if (isDivide(instructionName)) {
            operation = FlyweightOperation.DIVIDE;
        }

        if (operation != null) {
            flyweight.setOperationType(operation);
            return flyweight;
        }

        return instructionsFactory.createInstruction(parsedLine);
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
