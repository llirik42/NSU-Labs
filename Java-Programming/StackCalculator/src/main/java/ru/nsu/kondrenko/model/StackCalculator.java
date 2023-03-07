package ru.nsu.kondrenko.model;

import ru.nsu.kondrenko.model.context.Context;
import ru.nsu.kondrenko.model.exceptions.InstructionException;
import ru.nsu.kondrenko.model.instructions.Instruction;
import ru.nsu.kondrenko.model.instructionsfactory.InstructionsFactory;
import ru.nsu.kondrenko.model.parsing.AbstractParser;
import ru.nsu.kondrenko.model.parsing.ParsedLine;

import java.util.logging.Logger;

public class StackCalculator {
    private final AbstractParser parser;

    private final Logger logger;

    private final Context context;

    private final InstructionsFactory factory;

    public StackCalculator(AbstractParser parser, Logger logger, Context context, InstructionsFactory factory) {
        this.parser = parser;
        this.logger = logger;
        this.context = context;
        this.factory = factory;
    }

    public void calculate() {
        logger.info("Calculator started it's work");

        while (parser.hasNext() && !context.getExitFlag()) {
            ParsedLine parsedLine = parser.next();

            try {
                logger.info("Started reading line " + parsedLine.getLineNumber());

                Instruction instruction = factory.createInstruction(parsedLine.getInstructionName());

                if (context.getStackSize() < instruction.getNeededElementsCountOnStack()) {
                    throw new InstructionException("Not enough elements on the stack to execute the instruction");
                }
                if (parsedLine.getInstructionOperands().length != instruction.getNeededOperandsCount()) {
                    throw new InstructionException("Incorrect number of operands for the instruction");
                }

                logger.info("Started executing instruction");

                instruction.execute(context, parsedLine.getInstructionOperands());

                logger.info("Finished executing instruction");
            } catch (InstructionException exception) {
                Utilities.logException(logger, parsedLine.getLineNumber(), exception);
            }
        }

        logger.info("Calculator successfully finished it's work");
    }
}
