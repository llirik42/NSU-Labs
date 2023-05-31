package ru.nsu.kondrenko.common;

import java.io.Serializable;

public interface Message extends Serializable {
    String toXML();
}
