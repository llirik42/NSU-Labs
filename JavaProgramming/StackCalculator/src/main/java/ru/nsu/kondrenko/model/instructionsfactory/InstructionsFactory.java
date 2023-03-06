package ru.nsu.kondrenko.model.instructionsfactory;

import ru.nsu.kondrenko.model.instructions.Instruction;

public interface InstructionsFactory {
    String CONFIG_NAME = "factory_config.txt";

    Instruction createInstruction(String instructionName);
}
