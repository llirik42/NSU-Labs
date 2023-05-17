package ru.nsu.kondrenko.model;

public interface Visitor <T extends Number> {
    void visit(T x);

    T getResult();
}
