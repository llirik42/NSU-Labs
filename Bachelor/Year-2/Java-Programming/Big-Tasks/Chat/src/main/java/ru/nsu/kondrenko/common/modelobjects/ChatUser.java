package ru.nsu.kondrenko.common.modelobjects;

import java.io.Serializable;
import java.util.Objects;

public record ChatUser(String name, String type) implements Serializable {
    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }

        if (this.getClass() != o.getClass()) {
            return false;
        }

        return Objects.equals(((ChatUser) o).name, name);
    }

    @Override
    public int hashCode() {
        return name.hashCode();
    }
}
