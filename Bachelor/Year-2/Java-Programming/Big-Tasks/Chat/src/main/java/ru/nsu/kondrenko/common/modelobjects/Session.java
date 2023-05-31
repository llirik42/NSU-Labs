package ru.nsu.kondrenko.common.modelobjects;

import java.io.Serializable;
import java.util.Objects;
import java.util.concurrent.atomic.AtomicInteger;

public class Session implements Serializable {
    private static final AtomicInteger globalSessionId = new AtomicInteger(0);

    private final String id;

    public Session() {
        this.id = String.valueOf(globalSessionId.getAndIncrement());
    }

    public Session(String id) {
        this.id = id;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }

        if (getClass() != o.getClass()) {
            return false;
        }

        return Objects.equals(((Session) o).id, id);
    }

    @Override
    public int hashCode() {
        return id.hashCode();
    }

    @Override
    public String toString() {
        return id;
    }
}
