package ru.nsu.kondrenko.model.instructionsfactory;

import ru.nsu.kondrenko.model.exceptions.AbstractException;
import ru.nsu.kondrenko.model.exceptions.ConfigException;
import ru.nsu.kondrenko.model.exceptions.FileException;
import ru.nsu.kondrenko.model.exceptions.InstructionException;
import ru.nsu.kondrenko.model.instructions.Instruction;
import ru.nsu.kondrenko.model.parsing.ParsedLine;

import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.*;

public class InstructionsLoadingFactory implements InstructionsFactory {
    String CONFIG_NAME = "factory_config.txt";

    private final Map<String, Constructor<?>> nonLoggingInstructionsConstructors = new HashMap<>();

    private Constructor<Instruction> loggingInstructionConstructor;

    public InstructionsLoadingFactory() {
        try (InputStream configInputStream = InstructionsLoadingFactory.class.getClassLoader().getResourceAsStream(CONFIG_NAME)) {
            if (configInputStream == null) {
                throw new FileException("No instructions factory config was found");
            }

            readInstructions(configInputStream);
        } catch (SecurityException exception) {
            throw new ConfigException("Classes for the instructions factory can't be loaded because of the security", exception);
        } catch (NoSuchElementException exception) {
            throw new ConfigException("Incorrect format of the instructions factory config", exception);
        } catch (IllegalStateException exception) {
            throw new FileException("Error occurred during reading the instructions factory config", exception);
        } catch (LinkageError error) {
            throw new ConfigException("Error occurred during linkage classes for the instructions factory", error);
        } catch (ClassNotFoundException exception) {
            throw new ConfigException("Classes specified in the instructions factory config can't be located", exception);
        } catch (NoSuchMethodException exception) {
            throw new ConfigException("Classes specified in the instructions factory config don't have constructor " +
                    "with no parameters", exception);
        } catch (IOException exception) {
            throw new FileException("Error occurred during closing the instructions factory config", exception);
        }
    }

    @Override
    public Instruction createInstruction(ParsedLine parsedLine) {
        final String instructionName = parsedLine.getWords()[0];

        try {
            if (isLog(instructionName)) {
                Instruction instructionToDecorate = createNonLoggingInstruction(parsedLine.getWords()[1]);
                return createLogDecorator(instructionToDecorate);
            }

            if (!nonLoggingInstructionsConstructors.containsKey(instructionName)) {
                throw new InstructionException("Unknown instruction");
            }

            return createNonLoggingInstruction(instructionName);
        } catch (InvocationTargetException exception) {
            throw new AbstractException(exception);
        } catch (InstantiationException | IllegalAccessException | ExceptionInInitializerError exception) {
            throw new ConfigException("Classes specified in the instructions factory config are either incorrectly " +
                    "written or unavailable", exception);
        }
    }

    @Override
    public boolean isAdd(String instructionName) {
        return instructionName.equals("+");
    }

    @Override
    public boolean isMultiply(String instructionName) {
        return instructionName.equals("*");
    }

    @Override
    public boolean isSubtract(String instructionName) {
        return instructionName.equals("-");
    }

    @Override
    public boolean isDivide(String instructionName) {
        return instructionName.equals("-");
    }

    @Override
    public boolean isLog(String instructionName) {
        return instructionName.equals("LOG");
    }

    private Instruction createNonLoggingInstruction(String instructionName) throws InvocationTargetException, InstantiationException, IllegalAccessException {
        return (Instruction) nonLoggingInstructionsConstructors.get(instructionName).newInstance();
    }

    private Instruction createLogDecorator(Instruction instructionToLog) throws InvocationTargetException, InstantiationException, IllegalAccessException {
        return loggingInstructionConstructor.newInstance(instructionToLog);
    }

    private void readInstructions(InputStream inputStream) throws NoSuchElementException,
                                                                  IllegalStateException,
                                                                  LinkageError,
                                                                  ClassNotFoundException,
                                                                  SecurityException,
                                                                  NoSuchMethodException {
        final Class<?>[] parametersOfConstructorOfLoggingInstruction = new Class<?>[]{Instruction.class};

        Scanner scanner = new Scanner(inputStream);

        while (scanner.hasNext()) {
            String instructionName = scanner.next();
            String instructionPath = scanner.next();

            if (isLog(instructionName)) {
                loggingInstructionConstructor = (Constructor<Instruction>) Class.forName(instructionPath).getDeclaredConstructor(parametersOfConstructorOfLoggingInstruction);
            } else {
                nonLoggingInstructionsConstructors.put(instructionName, Class.forName(instructionPath).getConstructor());
            }
        }
    }
}
