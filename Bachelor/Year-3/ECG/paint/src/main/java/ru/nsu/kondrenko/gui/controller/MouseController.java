package ru.nsu.kondrenko.gui.controller;

import ru.nsu.kondrenko.model.context.Context;
import ru.nsu.kondrenko.model.context.ContextAction;
import ru.nsu.kondrenko.model.context.ContextTools;
import ru.nsu.kondrenko.model.image.ImageDrawing;

import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

public class MouseController extends MouseAdapter {
    private final Context context;

    private boolean hasPreviousCoordinates;
    private int prevX;
    private int prevY;

    public MouseController(Context context) {
        this.context = context;
    }

    @Override
    public void mousePressed(MouseEvent mouseEvent) {
        final ContextTools tool = context.getTool();

        if (tool == ContextTools.NONE) {
            return;
        }

        if (tool == ContextTools.DRAW_LINE) {
            handleDrawingLineState(mouseEvent);
        } else if (tool == ContextTools.DRAW_POLYGON) {
            handleDrawingPolygonState(mouseEvent);
        } else if (tool == ContextTools.DRAW_STAR) {
            handleDrawingStarState(mouseEvent);
        } else {
            handleFilling(mouseEvent);
        }

        context.setAction(ContextAction.REPAINT);
    }

    private void handleDrawingLineState(MouseEvent mouseEvent) {
        if (!hasPreviousCoordinates) {
            prevX = mouseEvent.getX();
            prevY = mouseEvent.getY();
        } else {
            drawLine(mouseEvent);
        }
        hasPreviousCoordinates = !hasPreviousCoordinates;
    }

    private void handleDrawingPolygonState(MouseEvent mouseEvent) {
        drawPolygon(mouseEvent);
    }

    private void handleDrawingStarState(MouseEvent mouseEvent) {
        drawStar(mouseEvent);
    }

    private void handleFilling(MouseEvent mouseEvent) {
        fill(mouseEvent);
    }

    private void drawLine(MouseEvent mouseEvent) {
        ImageDrawing.drawLine(
                context.getImage(),
                context.getColor(),
                context.getThickness(),
                prevX,
                prevY,
                mouseEvent.getX(),
                mouseEvent.getY()
        );
    }

    private void drawPolygon(MouseEvent mouseEvent) {
        ImageDrawing.drawPolygon(
                context.getImage(),
                context.getColor(),
                mouseEvent.getX(),
                mouseEvent.getY(),
                context.getThickness(),
                context.getNumberOfVertices(),
                context.getRadius(),
                context.getRotation()
        );
    }

    private void drawStar(MouseEvent mouseEvent) {
        ImageDrawing.drawStar(
                context.getImage(),
                context.getColor(),
                mouseEvent.getX(),
                mouseEvent.getY(),
                context.getThickness(),
                context.getNumberOfVertices(),
                context.getRadius(),
                context.getRotation()
        );
    }

    private void fill(MouseEvent mouseEvent) {
        ImageDrawing.fill(
                context.getImage(),
                context.getColor(),
                mouseEvent.getX(),
                mouseEvent.getY()
        );
    }
}
