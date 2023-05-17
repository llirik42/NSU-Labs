package ru.nsu.kondrenko.model;

import java.util.HashMap;
import java.util.Map;
import java.util.function.Function;

public class NumberVisitorsFactory<T extends Number> {
    private final Map<Enum<Mode>, Function<NumberVisitorsFactory<T>, Visitor<T>>> map = new HashMap<>();

    {
        map.put(Mode.SUMMATION, NumberVisitorsFactory::create_summator);
        map.put(Mode.MULTIPLICATION, NumberVisitorsFactory::create_multiplier);
    }

    public Visitor<T> create_number_visitor(Enum<Mode> mode) {
        return map.get(mode).apply(this);
    }

    private Visitor<T> create_summator() {
        return new Summator<>();
    }

    private Visitor<T> create_multiplier() {
        return new Multiplier<>();
    }
}
