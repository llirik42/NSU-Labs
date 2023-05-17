package ru.nsu.kondrenko.model.parsing.limits;

import ru.nsu.kondrenko.model.parsing.PropertiesException;
import ru.nsu.kondrenko.model.parsing.Utils;

import java.util.Properties;

public class DefaultLimitsParser implements LimitsParser {
    private static final String MIN_BODY_SUPPLIERS_COUNT_KEY = "MIN_BODY_SUPPLIERS_COUNT";
    private static final String MAX_BODY_SUPPLIERS_COUNT_KEY = "MAX_BODY_SUPPLIERS_COUNT";
    private static final String MIN_BODY_SUPPLY_TIME_MS_KEY = "MIN_BODY_SUPPLY_TIME_MS";
    private static final String MAX_BODY_SUPPLY_TIME_MS = "MAX_BODY_SUPPLY_TIME_MS";

    private static final String MIN_ENGINE_SUPPLIERS_COUNT_KEY = "MIN_ENGINE_SUPPLIERS_COUNT";
    private static final String MAX_ENGINE_SUPPLIERS_COUNT_KEY = "MAX_ENGINE_SUPPLIERS_COUNT";
    private static final String MIN_ENGINE_SUPPLY_TIME_MS_KEY = "MIN_ENGINE_SUPPLY_TIME_MS";
    private static final String MAX_ENGINE_SUPPLY_TIME_MS_KEY = "MAX_ENGINE_SUPPLY_TIME_MS";

    private static final String MIN_ACCESSORY_SUPPLIERS_COUNT_KEY = "MIN_ACCESSORY_SUPPLIERS_COUNT";
    private static final String MAX_ACCESSORY_SUPPLIERS_COUNT_KEY = "MAX_ACCESSORY_SUPPLIERS_COUNT";
    private static final String MIN_ACCESSORY_SUPPLY_TIME_MS_KEY = "MIN_ACCESSORY_SUPPLY_TIME_MS";
    private static final String MAX_ACCESSORY_SUPPLY_TIME_MS_KEY = "MAX_ACCESSORY_SUPPLY_TIME_MS";

    private static final String MIN_WORKERS_COUNT_KEY = "MIN_WORKERS_COUNT";
    private static final String MAX_WORKERS_COUNT_KEY = "MAX_WORKERS_COUNT";
    private static final String MIN_WORKERS_TIME_MS_KEY = "MIN_CAR_ASSEMBLING_TIME_MS";
    private static final String MAX_WORKERS_TIME_MS_KEY = "MAX_CAR_ASSEMBLING_TIME_MS";

    private static final String MIN_DEALERS_COUNT_KEY = "MIN_DEALERS_COUNT";
    private static final String MAX_DEALERS_COUNT_KEY = "MAX_DEALERS_COUNT";
    private static final String MIN_DEALERS_TIME_MS_KEY = "MIN_CAR_REQUEST_TIME_MS";
    private static final String MAX_DEALERS_TIME_MS_KEY = "MAX_CAR_REQUEST_TIME_MS";

    public Limits parse(Properties properties) throws PropertiesException {
        return new Limits(
                Utils.parseIntegerProperty(MIN_BODY_SUPPLIERS_COUNT_KEY, properties),
                Utils.parseIntegerProperty(MAX_BODY_SUPPLIERS_COUNT_KEY, properties),
                Utils.parseIntegerProperty(MIN_BODY_SUPPLY_TIME_MS_KEY, properties),
                Utils.parseIntegerProperty(MAX_BODY_SUPPLY_TIME_MS, properties),
                Utils.parseIntegerProperty(MIN_ENGINE_SUPPLIERS_COUNT_KEY, properties),
                Utils.parseIntegerProperty(MAX_ENGINE_SUPPLIERS_COUNT_KEY, properties),
                Utils.parseIntegerProperty(MIN_ENGINE_SUPPLY_TIME_MS_KEY, properties),
                Utils.parseIntegerProperty(MAX_ENGINE_SUPPLY_TIME_MS_KEY, properties),
                Utils.parseIntegerProperty(MIN_ACCESSORY_SUPPLIERS_COUNT_KEY, properties),
                Utils.parseIntegerProperty(MAX_ACCESSORY_SUPPLIERS_COUNT_KEY, properties),
                Utils.parseIntegerProperty(MIN_ACCESSORY_SUPPLY_TIME_MS_KEY, properties),
                Utils.parseIntegerProperty(MAX_ACCESSORY_SUPPLY_TIME_MS_KEY, properties),
                Utils.parseIntegerProperty(MIN_WORKERS_COUNT_KEY, properties),
                Utils.parseIntegerProperty(MAX_WORKERS_COUNT_KEY, properties),
                Utils.parseIntegerProperty(MIN_WORKERS_TIME_MS_KEY, properties),
                Utils.parseIntegerProperty(MAX_WORKERS_TIME_MS_KEY, properties),
                Utils.parseIntegerProperty(MIN_DEALERS_COUNT_KEY, properties),
                Utils.parseIntegerProperty(MAX_DEALERS_COUNT_KEY, properties),
                Utils.parseIntegerProperty(MIN_DEALERS_TIME_MS_KEY, properties),
                Utils.parseIntegerProperty(MAX_DEALERS_TIME_MS_KEY, properties)
        );
    }
}
