package ru.nsu.kondrenko.model;

import java.math.BigDecimal;

public class Summator<T extends Number> implements Visitor<T> {
    private T result;

    public Summator() {
        result = (T) BigDecimal.valueOf(0);
    }

    @Override
    public void visit(T x) {
        result = (T) BigDecimal.valueOf(result.doubleValue()).add(BigDecimal.valueOf(x.doubleValue()));
    }

    @Override
    public T getResult() {
        return result;
    }
}
