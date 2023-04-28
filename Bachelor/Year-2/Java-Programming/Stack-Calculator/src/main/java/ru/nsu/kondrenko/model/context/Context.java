package  ru.nsu.kondrenko.model.context;

public interface Context {
    void pushValue(double value);

    void pushVariable(String name);

    double pop();

    double peek();

    boolean isVariableName(String string);

    void defineVariable(String name, double value);

    int getStackSize();

    void setExitFlag();

    boolean getExitFlag();
}
