package ru.nsu.kondrenko.model.instructionsfactory;

import ru.nsu.kondrenko.model.instructions.Flyweight;
import ru.nsu.kondrenko.model.instructions.FlyweightOperation;
import ru.nsu.kondrenko.model.instructions.Instruction;

public class InstructionsFlyweightFactory implements InstructionsFactory {
    private final Flyweight flyweight = new Flyweight();

    private final InstructionsFactory instructionsFactory;

    public InstructionsFlyweightFactory(InstructionsFactory instructionsFactory) {
        this.instructionsFactory = instructionsFactory;
    }

    @Override
    public Instruction createInstruction(String instructionName) {
        if (instructionName.equals("*") || instructionName.equals("+") || instructionName.equals("-") || instructionName.equals("/")) {
            FlyweightOperation operation = null;

            if (instructionName.equals("*")) {
                operation = FlyweightOperation.MULTIPLY;
            }
            if (instructionName.equals("+")) {
                operation = FlyweightOperation.ADD;
            }
            if (instructionName.equals("/")) {
                operation = FlyweightOperation.DIVIDE;
            }
            if (instructionName.equals("-")) {
                operation = FlyweightOperation.SUBTRACT;
            }

            flyweight.setOperationType(operation);

            return flyweight;
        }

        return instructionsFactory.createInstruction(instructionName);
    }
}
