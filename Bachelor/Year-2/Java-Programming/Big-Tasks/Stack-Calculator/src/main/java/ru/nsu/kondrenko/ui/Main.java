package ru.nsu.kondrenko.ui;

import net.sourceforge.argparse4j.inf.ArgumentParser;
import net.sourceforge.argparse4j.ArgumentParsers;

import ru.nsu.kondrenko.model.StackCalculator;
import ru.nsu.kondrenko.model.context.Context;
import ru.nsu.kondrenko.model.context.ContextExceptionsDecorator;
import ru.nsu.kondrenko.model.context.RAMContext;
import ru.nsu.kondrenko.model.instructionsfactory.CachingInstructionsFactory;
import ru.nsu.kondrenko.model.instructionsfactory.InstructionsFactory;
import ru.nsu.kondrenko.model.instructionsfactory.InstructionsFlyweightFactory;
import ru.nsu.kondrenko.model.instructionsfactory.InstructionsLoadingFactory;
import ru.nsu.kondrenko.model.parsersfactory.DefaultParsersFactory;
import ru.nsu.kondrenko.model.parsersfactory.ParsersFactory;
import ru.nsu.kondrenko.model.Utilities;
import ru.nsu.kondrenko.model.parsing.AbstractParser;

import java.io.IOException;
import java.io.InputStream;
import java.util.logging.LogManager;
import java.util.logging.Logger;

public class Main {
    private static final String LOGGING_CONFIGURATION_FILE_NAME = "logging.properties";

    public static void main(String[] args) {
        ArgumentParser argumentParser = createArgumentParser();

        readLoggingConfigurationFile();

        Logger logger = Logger.getLogger("global_logger");

        String inputPath = argumentParser.parseArgsOrFail(args).get("file");

        InstructionsFactory loadingFactory = new InstructionsLoadingFactory();
        InstructionsFactory cachingFactory = new CachingInstructionsFactory(loadingFactory);
        InstructionsFactory flyweightFactory = new InstructionsFlyweightFactory(cachingFactory);

        Context context = new RAMContext();
        Context contextDecorator = new ContextExceptionsDecorator(context);

        ParsersFactory parsersFactory = new DefaultParsersFactory();

        try (AbstractParser parser = parsersFactory.createParser(inputPath)) {
            var calculator = new StackCalculator(parser, logger, contextDecorator, flyweightFactory);
            calculator.calculate();
        } catch (Exception exception) {
            Utilities.logException(logger, exception);
            System.err.println(exception.getLocalizedMessage());
        }
    }

    private static ArgumentParser createArgumentParser() {
        ArgumentParser argumentParser = ArgumentParsers.newFor("Stack calculator")
                .build()
                .epilog(Utilities.getHelp());

        argumentParser.addArgument("-f", "--file")
                .type(String.class)
                .help("файл с инструкциями для калькулятора");

        return argumentParser;
    }

    private static void readLoggingConfigurationFile() {
        try (InputStream loggingConfigFile = Utilities.class.getClassLoader().getResourceAsStream(LOGGING_CONFIGURATION_FILE_NAME)) {
            if (loggingConfigFile == null) {
                System.err.println("No logging configuration file was found");
                System.exit(1);
            }

            LogManager.getLogManager().readConfiguration(loggingConfigFile);
        } catch (IOException exception) {
            System.err.println("Error occurred during reading/closing logging configuration file");
            System.exit(1);
        } catch (SecurityException exception) {
            System.err.println("No permissions to read the logging configuration file");
            System.exit(1);
        }
    }
}
