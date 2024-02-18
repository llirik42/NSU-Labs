package ru.nsu.kondrenko.model.context;

import java.awt.*;
import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.List;

public class ContextImpl implements Context {
    private final List<ContextListener> listeners = new ArrayList<>();

    private ContextTools tool;
    private ContextAction state;
    private BufferedImage image;
    private Color color;
    private String errorMessage;
    private int thickness;
    private int radius;
    private int numberOfVertices;
    private int rotation;

    public ContextImpl(BufferedImage image, Color color, int thickness, int radius, int numberOfVertices, int rotation) {
        this.tool = ContextTools.NONE;
        this.state = ContextAction.IDLE;
        this.image = image;
        this.color = color;
        this.thickness = thickness;
        this.radius = radius;
        this.numberOfVertices = numberOfVertices;
        this.rotation = rotation;
    }

    @Override
    public void addListener(ContextListener listener) {
        listeners.add(listener);
    }

    @Override
    public void removeListener(ContextListener listener) {
        listeners.remove(listener);
    }

    @Override
    public void notifyListenersAboutActionChange() {
        for (final var it : listeners) {
            it.onContextActionChange(this);
        }
    }

    @Override
    public void notifyListenersAboutToolChange() {
        for (final var it : listeners) {
            it.onContextToolChange(this);
        }
    }

    @Override
    public ContextAction getAction() {
        return state;
    }

    @Override
    public void setAction(ContextAction action) {
        this.state = action;
        notifyListenersAboutActionChange();
    }

    @Override
    public ContextTools getTool() {
        return tool;
    }

    @Override
    public void setTool(ContextTools tool) {
        this.tool = tool;
        notifyListenersAboutToolChange();
    }

    @Override
    public BufferedImage getImage() {
        return image;
    }

    @Override
    public void setImage(BufferedImage image) {
        this.image = image;
    }

    @Override
    public int getThickness() {
        return thickness;
    }

    @Override
    public void setThickness(int thickness) {
        this.thickness = thickness;
    }

    @Override
    public Color getColor() {
        return color;
    }

    @Override
    public void setColor(Color color) {
        this.color = color;
    }

    @Override
    public int getNumberOfVertices() {
        return numberOfVertices;
    }

    @Override
    public void setNumberOfVertices(int numberOfVertices) {
        this.numberOfVertices = numberOfVertices;
    }

    @Override
    public int getRadius() {
        return radius;
    }

    @Override
    public void setRadius(int radius) {
        this.radius = radius;
    }

    @Override
    public int getRotation() {
        return rotation;
    }

    @Override
    public void setRotation(int rotation) {
        this.rotation = rotation;
    }

    @Override
    public String getErrorMessage() {
        return errorMessage;
    }

    @Override
    public void setErrorMessage(String errorMessage) {
        this.errorMessage = errorMessage;
    }

    @Override
    public void pushError(String errorMessage) {
        setErrorMessage(errorMessage);
        setAction(ContextAction.HANDLE_ERROR);
    }
}
