package ru.nsu.kondrenko.model;

import java.math.BigDecimal;

public class Multiplier<T extends Number> implements Visitor<T> {
    private T result;

    public Multiplier() {
        result = (T) BigDecimal.valueOf(1);
    }

    @Override
    public void visit(T x) {
        result = (T) BigDecimal.valueOf(result.doubleValue()).multiply(BigDecimal.valueOf(x.doubleValue()));
    }

    @Override
    public T getResult() {
        return result;
    }
}
