package ru.nsu.kondrenko.model.instructionsfactory;

import ru.nsu.kondrenko.model.exceptions.AbstractException;
import ru.nsu.kondrenko.model.exceptions.ConfigException;
import ru.nsu.kondrenko.model.exceptions.FileException;
import ru.nsu.kondrenko.model.exceptions.InstructionException;
import ru.nsu.kondrenko.model.instructions.Instruction;

import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.HashMap;
import java.util.Map;
import java.util.NoSuchElementException;
import java.util.Scanner;

public class InstructionsLoadingFactory implements InstructionsFactory {
    private final Map<String, Constructor<?>> instructionsConstructors = new HashMap<>();

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
    public Instruction createInstruction(String instructionName) {
        if (!instructionsConstructors.containsKey(instructionName)) {
            throw new InstructionException("Unknown instruction");
        }

        try {
            return (Instruction) instructionsConstructors.get(instructionName).newInstance();
        } catch (InvocationTargetException exception) {
            throw new AbstractException(exception);
        } catch (InstantiationException | IllegalAccessException | ExceptionInInitializerError exception) {
            throw new ConfigException("Classes specified in the instructions factory config are either incorrectly " +
                    "written or unavailable", exception);
        }
    }

    private void readInstructions(InputStream inputStream) throws NoSuchElementException,
                                                                  IllegalStateException,
                                                                  LinkageError,
                                                                  ClassNotFoundException,
                                                                  SecurityException,
                                                                  NoSuchMethodException {
        Scanner scanner = new Scanner(inputStream);

        while (scanner.hasNext()) {
            String instructionName = scanner.next();
            String instructionPath = scanner.next();
            instructionsConstructors.put(instructionName, Class.forName(instructionPath).getConstructor());
        }
    }
}
