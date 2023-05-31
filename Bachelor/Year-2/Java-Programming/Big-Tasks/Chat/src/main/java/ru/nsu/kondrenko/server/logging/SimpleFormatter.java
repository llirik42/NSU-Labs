package ru.nsu.kondrenko.server.logging;

import java.util.logging.Formatter;
import java.util.logging.LogRecord;

public class SimpleFormatter extends Formatter {
    @Override
    public String format(LogRecord logRecord) {
        return String.format(
                "%s: %s%n%n",
                logRecord.getLevel(),
                logRecord.getMessage()
        );
    }
}
