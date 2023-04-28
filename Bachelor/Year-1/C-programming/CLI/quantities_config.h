#pragma once

#include "conversion.h"
#include "quantities_group.h"

static const struct QuantityGroup TEMPERATURE_GROUP = {
        .title = "temperature",
        .basicQuantity = {.title = "Celsius", .minValue = -273.15},
        .nonBasicQuantities = {
                {.title = "Kelvin", .toBasic = &kelvinToCelsius,.fromBasic = &celsiusToKelvin},

                {.title = "Fahrenheit", .toBasic = &fahrenheitToCelsius,
                 .fromBasic = &celsiusToFahrenheit}
        },
        .quantitiesCounter = 3
};

static const struct QuantityGroup DISTANCE_GROUP = {
        .title = "distance",
        .basicQuantity = {.title = "metres", .minValue = 0},
        .nonBasicQuantities = {
                {.title = "feet", .toBasic = &feetToMeters, .fromBasic = &metersToFeet},

                {.title = "miles", .toBasic = &milesToMeters, .fromBasic = metersToMiles}
        },
        .quantitiesCounter = 3
};

static const struct QuantityGroup WEIGHT_GROUP = {
        .title = "weight",
        .basicQuantity = {.title = "grams", .minValue = 0},
        .nonBasicQuantities = {
                {.title = "pounds", .toBasic = &poundsToGrams, .fromBasic = &gramsToPounds},

                {.title = "ounces", .toBasic = &ouncesToGrams, .fromBasic = &gramsToOunces},
        },
        .quantitiesCounter = 3
};


static const struct QuantityGroup* GROUP_POINTERS[] = {&TEMPERATURE_GROUP, &DISTANCE_GROUP, &WEIGHT_GROUP};
static const unsigned int GROUPS_COUNTER = sizeof(GROUP_POINTERS) / sizeof(GROUP_POINTERS[0]);
