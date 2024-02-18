package ru.nsu.kondrenko.gui.view;

import ru.nsu.kondrenko.model.image.ImageReaderImpl;

import javax.swing.*;
import java.io.IOException;

public class ToolsIconsSupplierImpl implements ToolsIconsSupplier {
    private final ImageReaderImpl reader;

    private final ImageIcon drawLineIcon;
    private final ImageIcon drawPolygonIcon;
    private final ImageIcon drawStarIcon;
    private final ImageIcon fillIcon;
    private final ImageIcon clearIcon;
    private final ImageIcon selectColorIcon;
    private final ImageIcon selectThicknessIcon;
    private final ImageIcon selectNumberOfSidesIcon;
    private final ImageIcon selectRadiusIcon;
    private final ImageIcon selectRotationIcon;
    private final ImageIcon blackIcon;
    private final ImageIcon whiteIcon;
    private final ImageIcon redIcon;
    private final ImageIcon greenIcon;
    private final ImageIcon blueIcon;
    private final ImageIcon magentaIcon;
    private final ImageIcon pinkIcon;
    private final ImageIcon orangeIcon;
    private final ImageIcon yellowIcon;
    private final ImageIcon cyanIcon;

    public ToolsIconsSupplierImpl() throws IOException {
        reader = new ImageReaderImpl();
        drawLineIcon = loadImageIcon("/icons/line.png");
        drawPolygonIcon = loadImageIcon("/icons/polygon.png");
        drawStarIcon = loadImageIcon("/icons/star.png");
        fillIcon = loadImageIcon("/icons/fill.png");
        clearIcon = loadImageIcon("/icons/clear.png");
        selectColorIcon = loadImageIcon("/icons/palette.png");
        selectThicknessIcon = loadImageIcon("/icons/thickness.png");
        selectNumberOfSidesIcon = loadImageIcon("/icons/shapes.png");
        selectRadiusIcon = loadImageIcon("/icons/radius.png");
        selectRotationIcon = loadImageIcon("/icons/rotation.png");

        blackIcon = loadColorIcon("black");
        whiteIcon = loadColorIcon("white");
        redIcon = loadColorIcon("red");
        greenIcon = loadColorIcon("green");
        blueIcon = loadColorIcon("blue");
        magentaIcon = loadColorIcon("magenta");
        pinkIcon = loadColorIcon("pink");
        orangeIcon = loadColorIcon("orange");
        yellowIcon = loadColorIcon("yellow");
        cyanIcon = loadColorIcon("cyan");
    }

    @Override
    public ImageIcon getDrawLineIcon() {
        return drawLineIcon;
    }

    @Override
    public ImageIcon getDrawPolygonIcon() {
        return drawPolygonIcon;
    }

    @Override
    public ImageIcon getDrawStarIcon() {
        return drawStarIcon;
    }

    @Override
    public ImageIcon getFillIcon() {
        return fillIcon;
    }

    @Override
    public ImageIcon getClearIcon() {
        return clearIcon;
    }

    @Override
    public ImageIcon getSelectColorIcon() {
        return selectColorIcon;
    }

    @Override
    public ImageIcon getSelectThicknessIcon() {
        return selectThicknessIcon;
    }

    @Override
    public ImageIcon getSelectNumberOfVerticesIcon() {
        return selectNumberOfSidesIcon;
    }

    @Override
    public ImageIcon getSelectRadiusIcon() {
        return selectRadiusIcon;
    }

    @Override
    public ImageIcon getSelectRotationIcon() {
        return selectRotationIcon;
    }

    @Override
    public ImageIcon getBlackIcon() {
        return blackIcon;
    }

    @Override
    public ImageIcon getWhiteIcon() {
        return whiteIcon;
    }

    @Override
    public ImageIcon getRedIcon() {
        return redIcon;
    }

    @Override
    public ImageIcon getGreenIcon() {
        return greenIcon;
    }

    @Override
    public ImageIcon getBlueIcon() {
        return blueIcon;
    }

    @Override
    public ImageIcon getMagentaIcon() {
        return magentaIcon;
    }

    @Override
    public ImageIcon getPinkIcon() {
        return pinkIcon;
    }

    @Override
    public ImageIcon getOrangeIcon() {
        return orangeIcon;
    }

    @Override
    public ImageIcon getYellowIcon() {
        return yellowIcon;
    }

    @Override
    public ImageIcon getCyanIcon() {
        return cyanIcon;
    }

    private ImageIcon loadColorIcon(final String colorName) throws IOException {
        return loadImageIcon(String.format("/colors/%s.png", colorName));
    }

    private ImageIcon loadImageIcon(final String path) throws IOException {
        return new ImageIcon(reader.readAsResource(path));
    }
}
