package ru.nsu.kondrenko.model.instructionsfactory;

import ru.nsu.kondrenko.model.instructions.Instruction;

import java.util.HashMap;
import java.util.Map;

public class CachingInstructionsFactory implements InstructionsFactory {
    private final Map<String, Instruction> cachedInstructions = new HashMap<>();

    private final InstructionsFactory factory;

    public CachingInstructionsFactory() {
        this.factory = new InstructionsLoadingFactory();
    }

    @Override
    public Instruction createInstruction(String instructionName) {
        Instruction instruction;

        if (!cachedInstructions.containsKey(instructionName)) {
            instruction = factory.createInstruction(instructionName);
            cachedInstructions.put(instructionName, instruction);
        } else {
            instruction = cachedInstructions.get(instructionName);
        }

        return instruction;
    }
}
