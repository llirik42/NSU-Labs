package ru.nsu.kondrenko;

import ru.nsu.kondrenko.controller.*;
import ru.nsu.kondrenko.model.Emulator;
import ru.nsu.kondrenko.model.parsing.FilePropertiesReader;
import ru.nsu.kondrenko.model.parsing.PropertiesException;
import ru.nsu.kondrenko.model.parsing.PropertiesReader;
import ru.nsu.kondrenko.model.parsing.config.*;
import ru.nsu.kondrenko.model.parsing.limits.*;
import ru.nsu.kondrenko.view.AbstractFrameFactory;
import ru.nsu.kondrenko.view.DefaultFrameFactory;
import ru.nsu.kondrenko.view.GUI;

import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;
import java.util.logging.LogManager;
import java.util.logging.Logger;
import java.util.logging.SimpleFormatter;
import java.util.logging.StreamHandler;

public class Main {
    private static final String DEALERS_LOGGING_CONFIGURATION_FILE_NAME = "dealers_logging.properties";
    private static final String LIMITS_CONFIG_NAME = "limits.properties";
    private static final String FACTORY_CONFIG_NAME = "factory_config.properties";

    private static final int ERROR_CODE = 1;

    public static void main(String[] args) {
        final Logger errorsLogger = Logger.getAnonymousLogger();

        try {
            final PropertiesReader limitsPropertiesReader = new FilePropertiesReader(LIMITS_CONFIG_NAME);
            final Properties limitsProperties = limitsPropertiesReader.read();
            final LimitsParser limitsParser = new DefaultLimitsParser();
            final Limits limits = limitsParser.parse(limitsProperties);
            final LimitsValidator limitsValidator = new DefaultLimitsValidator();
            if (!limitsValidator.validate(limits)) {
                final String message = String.format("Incorrect file — %s%n", LIMITS_CONFIG_NAME);
                errorsLogger.severe(message);
                System.exit(ERROR_CODE);
            }

            final PropertiesReader configPropertiesReader = new FilePropertiesReader(FACTORY_CONFIG_NAME);
            final Properties configProperties = configPropertiesReader.read();
            final ConfigParser configParser = new DefaultConfigParser();
            final Config config = configParser.parse(configProperties);
            final ConfigValidator configValidator = new DefaultConfigValidator();
            if (!configValidator.validate(config, limits)) {
                final String message = String.format("Incorrect file — %s%n", FACTORY_CONFIG_NAME);
                errorsLogger.severe(message);
                System.exit(ERROR_CODE);
            }

            final Logger dealersLogger;
            if (!config.logging()) {
                dealersLogger = null;
            } else {
                dealersLogger = readLoggingConfigurationFile(errorsLogger) ? Logger.getAnonymousLogger() : null;
            }

            final BodySuppliersController bodySuppliersController = new BodySuppliersController();
            final EngineSuppliersController engineSuppliersController = new EngineSuppliersController();
            final AccessorySuppliersController accessorySuppliersController = new AccessorySuppliersController();
            final WorkersController workersController = new WorkersController();
            final DealersController dealersController = new DealersController();
            final BodySupplyTimeController bodySupplyTimeController = new BodySupplyTimeController();
            final EngineSupplyTmeController engineSupplyTmeController = new EngineSupplyTmeController();
            final AccessorySupplyTimeController accessorySupplyTimeController = new AccessorySupplyTimeController();
            final CarAssemblingTimeController carAssemblingTimeController = new CarAssemblingTimeController();
            final CarRequestTimeController carRequestTimeController = new CarRequestTimeController();
            final WindowClosingController windowClosingController = new WindowClosingController(errorsLogger);

            final AbstractFrameFactory frameFactory = new DefaultFrameFactory();
            final GUI gui = new GUI(
                    frameFactory,
                    config,
                    limits,
                    errorsLogger,
                    bodySuppliersController,
                    engineSuppliersController,
                    accessorySuppliersController,
                    workersController,
                    dealersController,
                    bodySupplyTimeController,
                    engineSupplyTmeController,
                    accessorySupplyTimeController,
                    carAssemblingTimeController,
                    carRequestTimeController,
                    windowClosingController
            );

            final Emulator emulator = new Emulator(config, dealersLogger, gui, gui, gui, gui, gui, gui);

            bodySuppliersController.setEmulator(emulator);
            engineSuppliersController.setEmulator(emulator);
            accessorySuppliersController.setEmulator(emulator);
            workersController.setEmulator(emulator);
            dealersController.setEmulator(emulator);
            bodySupplyTimeController.setEmulator(emulator);
            engineSupplyTmeController.setEmulator(emulator);
            accessorySupplyTimeController.setEmulator(emulator);
            carAssemblingTimeController.setEmulator(emulator);
            carRequestTimeController.setEmulator(emulator);
            windowClosingController.setEmulator(emulator);

            emulator.start();
        } catch (PropertiesException propertiesException) {
            errorsLogger.severe(propertiesException.getLocalizedMessage());
            System.exit(ERROR_CODE);
        }
    }

    private static boolean readLoggingConfigurationFile(Logger errorsLogger) {
        try (InputStream loggingConfigFile = Main.class.getClassLoader().getResourceAsStream(DEALERS_LOGGING_CONFIGURATION_FILE_NAME)) {
            if (loggingConfigFile == null) {
                throw new NullPointerException();
            }

            LogManager.getLogManager().readConfiguration(loggingConfigFile);
            final SimpleFormatter simpleFormatter = new SimpleFormatter();
            final StreamHandler streamHandler = new StreamHandler(System.err, simpleFormatter);
            errorsLogger.addHandler(streamHandler);

            return true;
        } catch (IOException exception) {
            errorsLogger.warning("Error occurred during opening logging configuration file");
        } catch (SecurityException exception) {
            errorsLogger.warning("No permissions to read the logging configuration file");
        } catch (NullPointerException exception) {
            errorsLogger.warning("No logging configuration file was found");
        }

        return false;
    }
}
