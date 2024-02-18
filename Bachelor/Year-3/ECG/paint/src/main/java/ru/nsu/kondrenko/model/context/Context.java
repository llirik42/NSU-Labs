package ru.nsu.kondrenko.model.context;

import java.awt.*;
import java.awt.image.BufferedImage;

public interface Context {
    void addListener(ContextListener listener);

    void removeListener(ContextListener listener);

    void notifyListenersAboutActionChange();

    void notifyListenersAboutToolChange();

    ContextAction getAction();

    void setAction(ContextAction action);

    ContextTools getTool();

    void setTool(ContextTools tool);

    BufferedImage getImage();

    void setImage(BufferedImage image);

    int getThickness();

    void setThickness(int thickness);

    Color getColor();

    void setColor(Color color);

    int getNumberOfVertices();

    void setNumberOfVertices(int numberOfVertices);

    int getRadius();

    void setRadius(int radius);

    int getRotation();

    void setRotation(int rotation);

    String getErrorMessage();

    void setErrorMessage(String errorMessage);

    void pushError(String errorMessage);
}
