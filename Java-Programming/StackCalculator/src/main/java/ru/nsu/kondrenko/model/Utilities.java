package ru.nsu.kondrenko.model;

import ru.nsu.kondrenko.model.exceptions.AbstractException;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.Objects;
import java.util.logging.*;

public final class Utilities {
    private Utilities() {}

    public static String getHelp() {
        return "Программа представляет собой стековый калькулятор, использующий фиксированный набор " +
                "операций над вещественными числами. Принимает на вход входной файл, в котором написаны " +
                "инструкции, разделённые символом новой строки (при этом операнды в инструкции разделены пробелом)." +
                "В случае отсутствия аргумента использует стандартный поток ввода для чтения инструкций. При вводе " +
                "инструкций из стандартного потока необходимо указать команду, отвечающую за конец ввода." +

                """
              
                                        
                Список доступных инструкций:
                  #                      строка с комментарием
                  POP                    снять число со стека
                  PUSH                   положить число на стек
                  +, -, /, *, SQRT       арифметические операции
                  PRINT                  печать верхнего элемента стека (без удаления)
                  DEFINE                 задать значение переменной
                  HELP                   помощь
                  EXIT                   закончить выполнение
                  
                Пример (для чтения из файла, должно вывести 2):
                  DEFINE a 4
                  
                  # Some comment
                  
                  PUSH a
                  SQRT
                  PRINT
                
                Пример (аналог для чтения из потока ввода, должно вывести 2):
                  DEFINE a 4
                  
                  # Some comment
                  
                  PUSH a
                  SQRT
                  PRINT
                  EXIT
                
                """;
    }

    public static void logException(Logger logger, Exception exception) {
        logException(logger, exception, exception.getLocalizedMessage());
    }

    public static void logException(Logger logger, int lineNumber, Exception exception) {
        logException(logger, exception, exception.getLocalizedMessage() + " on a line " + lineNumber);
    }

    public static String reduceSpaces(String string) {
        return string.trim().replaceAll(" +", " ");
    }

    public static boolean isLineFictive(String string, String commentPrefix) {
        return string.isBlank() || isLineComment(string, commentPrefix);
    }

    private static boolean isLineComment(String string, String commentPrefix) {
        return string.trim().startsWith(commentPrefix);
    }

    private static void logException(Logger logger, Exception exception, String newMessage) {
        try {
            var exceptionToBePrinted = Objects.equals(newMessage, exception.getLocalizedMessage())
                    ? exception
                    : exception.getClass().getConstructor(String.class, Throwable.class).newInstance(newMessage, exception.getCause());

            StringWriter stringWriter = new StringWriter();
            PrintWriter printWriter = new PrintWriter(stringWriter);
            exceptionToBePrinted.printStackTrace(printWriter);
            String stackTrace = stringWriter.toString();

            logger.severe(stackTrace);
        } catch (Exception e) {
            throw new AbstractException(e);
        }
    }
}
