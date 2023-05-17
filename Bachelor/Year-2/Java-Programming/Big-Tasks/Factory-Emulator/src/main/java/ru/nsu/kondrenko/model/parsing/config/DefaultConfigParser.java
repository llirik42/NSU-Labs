package ru.nsu.kondrenko.model.parsing.config;

import ru.nsu.kondrenko.model.parsing.PropertiesException;
import ru.nsu.kondrenko.model.parsing.Utils;

import java.util.Properties;

public class DefaultConfigParser implements ConfigParser {
    private static final String BODY_STORAGE_CAPACITY_KEY = "BODY_STORAGE_CAPACITY";
    private static final String ENGINE_STORAGE_CAPACITY_KEY = "ENGINE_STORAGE_CAPACITY";
    private static final String ACCESSORY_STORAGE_CAPACITY_KEY = "ACCESSORY_STORAGE_CAPACITY";
    private static final String CAR_STORAGE_CAPACITY_KEY = "CAR_STORAGE_CAPACITY";

    private static final String BODY_SUPPLIERS_COUNT_KEY = "BODY_SUPPLIERS_COUNT";
    private static final String ENGINE_SUPPLIERS_COUNT_KEY = "ENGINE_SUPPLIERS_COUNT";
    private static final String ACCESSORY_SUPPLIERS_COUNT_KEY = "ACCESSORY_SUPPLIERS_COUNT";
    private static final String WORKERS_COUNT_KEY = "WORKERS_COUNT";
    private static final String DEALERS_COUNT_KEY = "DEALERS_COUNT";

    private static final String BODY_SUPPLY_TIME_MS_KEY = "BODY_SUPPLY_TIME_MS";
    private static final String ENGINE_SUPPLY_TIME_MS_KEY = "ENGINE_SUPPLY_TIME_MS";
    private static final String ACCESSORY_SUPPLY_TIME_MS_KEY = "ACCESSORY_SUPPLY_TIME_MS";
    private static final String CAR_ASSEMBLING_TIME_MS_KEY = "CAR_ASSEMBLING_TIME_MS";
    private static final String CAR_REQUEST_TIME_MS_KEY = "CAR_REQUEST_TIME_MS";

    private static final String LOGGING_KEY = "LOGGING";

    @Override
    public Config parse(Properties properties) throws PropertiesException {
        return new Config(
                Utils.parseIntegerProperty(BODY_STORAGE_CAPACITY_KEY, properties),
                Utils.parseIntegerProperty(ENGINE_STORAGE_CAPACITY_KEY, properties),
                Utils.parseIntegerProperty(ACCESSORY_STORAGE_CAPACITY_KEY, properties),
                Utils.parseIntegerProperty(CAR_STORAGE_CAPACITY_KEY, properties),
                Utils.parseIntegerProperty(BODY_SUPPLIERS_COUNT_KEY, properties),
                Utils.parseIntegerProperty(ENGINE_SUPPLIERS_COUNT_KEY, properties),
                Utils.parseIntegerProperty(ACCESSORY_SUPPLIERS_COUNT_KEY, properties),
                Utils.parseIntegerProperty(WORKERS_COUNT_KEY, properties),
                Utils.parseIntegerProperty(DEALERS_COUNT_KEY, properties),
                Utils.parseIntegerProperty(BODY_SUPPLY_TIME_MS_KEY, properties),
                Utils.parseIntegerProperty(ENGINE_SUPPLY_TIME_MS_KEY, properties),
                Utils.parseIntegerProperty(ACCESSORY_SUPPLY_TIME_MS_KEY, properties),
                Utils.parseIntegerProperty(CAR_ASSEMBLING_TIME_MS_KEY, properties),
                Utils.parseIntegerProperty(CAR_REQUEST_TIME_MS_KEY, properties),
                Utils.parseBooleanProperty(LOGGING_KEY, properties)
        );
    }
}
