package ru.nsu.kondrenko;

import ru.nsu.kondrenko.controller.*;
import ru.nsu.kondrenko.model.Emulator;
import ru.nsu.kondrenko.model.parsing.FilePropertiesReader;
import ru.nsu.kondrenko.model.parsing.PropertiesException;
import ru.nsu.kondrenko.model.parsing.PropertiesReader;
import ru.nsu.kondrenko.model.parsing.config.*;
import ru.nsu.kondrenko.model.parsing.limits.*;
import ru.nsu.kondrenko.view.DefaultFrameFactory;
import ru.nsu.kondrenko.view.GUI;

import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;
import java.util.logging.LogManager;
import java.util.logging.Logger;

public class Main {
    private static final String LOGGING_CONFIGURATION_FILE_NAME = "logging.properties";
    private static final String LIMITS_CONFIG_NAME = "limits.properties";
    private static final String FACTORY_CONFIG_NAME = "factory_config.properties";

    private static final int ERROR_CODE = 1;

    public static void main(String[] args) {
        try {
            readLoggingConfigurationFile();

            final PropertiesReader limitsPropertiesReader = new FilePropertiesReader(LIMITS_CONFIG_NAME);
            final Properties limitsProperties = limitsPropertiesReader.read();
            final LimitsParser limitsParser = new DefaultLimitsParser();
            final Limits limits = limitsParser.parse(limitsProperties);
            final LimitsValidator limitsValidator = new DefaultLimitsValidator();
            if (!limitsValidator.validate(limits)) {
                System.err.printf("Incorrect file — %s%n", LIMITS_CONFIG_NAME);
                System.exit(ERROR_CODE);
            }

            final PropertiesReader configPropertiesReader = new FilePropertiesReader(FACTORY_CONFIG_NAME);
            final Properties configProperties = configPropertiesReader.read();
            final ConfigParser configParser = new DefaultConfigParser();
            final Config config = configParser.parse(configProperties);
            final ConfigValidator configValidator = new DefaultConfigValidator();
            if (!configValidator.validate(config, limits)) {
                System.err.printf("Incorrect file — %s%n", FACTORY_CONFIG_NAME);
                System.exit(ERROR_CODE);
            }

            final Logger logger = config.logging() ? Logger.getAnonymousLogger() : null;

            final Emulator emulator = new Emulator(config, logger);

            final BodySuppliersController bodySuppliersController = new BodySuppliersController(emulator);
            final EngineSuppliersController engineSuppliersController = new EngineSuppliersController(emulator);
            final AccessorySuppliersController accessorySuppliersController = new AccessorySuppliersController(emulator);
            final WorkersController workersController = new WorkersController(emulator);
            final DealersController dealersController = new DealersController(emulator);
            final BodySupplyTimeController bodySupplyTimeController = new BodySupplyTimeController(emulator);
            final EngineSupplyTmeController engineSupplyTmeController = new EngineSupplyTmeController(emulator);
            final AccessorySupplyTimeController accessorySupplyTimeController = new AccessorySupplyTimeController(emulator);
            final CarAssemblingTimeController carAssemblingTimeController = new CarAssemblingTimeController(emulator);
            final CarRequestTimeController carRequestTimeController = new CarRequestTimeController(emulator);
            final WindowClosingController windowClosingController = new WindowClosingController(emulator);

            final GUI gui = new GUI(
                    new DefaultFrameFactory(),
                    config,
                    limits,
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

            emulator.setView(gui);
            emulator.setSupplyListener(gui);
            emulator.setCarAssembleListener(gui);
            emulator.setCarRequestListener(gui);
            emulator.setStorageListener(gui);
            emulator.setFactoryTasksListener(gui);

            emulator.start();
        } catch (PropertiesException propertiesException) {
            System.err.println(propertiesException.getLocalizedMessage());
            System.exit(ERROR_CODE);
        }
    }

    private static void readLoggingConfigurationFile() {
        try (InputStream loggingConfigFile = Main.class.getClassLoader().getResourceAsStream(LOGGING_CONFIGURATION_FILE_NAME)) {
            LogManager.getLogManager().readConfiguration(loggingConfigFile);
        } catch (IOException exception) {
            System.err.println("Error occurred during opening logging configuration file");
            System.exit(ERROR_CODE);
        } catch (SecurityException exception) {
            System.err.println("No permissions to read the logging configuration file");
            System.exit(ERROR_CODE);
        } catch (NullPointerException exception) {
            System.err.println("No logging configuration file was found");
            System.exit(ERROR_CODE);
        }
    }
}
