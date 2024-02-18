package ru.nsu.kondrenko.model.image;

import java.awt.*;
import java.awt.geom.Path2D;
import java.awt.image.BufferedImage;
import java.util.ArrayDeque;
import java.util.Deque;

public final class ImageDrawing {
    private ImageDrawing() {
    }

    public static void drawLine(BufferedImage image, Color color, int thickness, int x1, int y1, int x2, int y2) {
        if (thickness == 1) {
            drawThinLine(image, color, x1, y1, x2, y2);
        } else {
            final Graphics2D graphics2D = (Graphics2D) image.getGraphics();
            graphics2D.setColor(color);
            graphics2D.setStroke(new BasicStroke(thickness));
            graphics2D.drawLine(x1, y1, x2, y2);
        }
    }

    public static void drawPolygon(BufferedImage image, Color color, int x, int y, int thickness, int n, int radius, int rotationDegrees) {
        final Polygon polygon = createPolygon(x, y, n, radius, rotationDegrees);
        final Graphics2D graphics2D = (Graphics2D) image.getGraphics();
        graphics2D.setColor(color);
        graphics2D.setStroke(new BasicStroke(thickness));
        graphics2D.drawPolygon(polygon);
    }

    public static void drawStar(BufferedImage image, Color color, int x, int y, int thickness, int n, int radius, int rotationDegrees) {
        final double innerRadius = (double) radius / 2;
        final Shape starShape = createStarShape(x, y, innerRadius, radius, n, rotationDegrees);
        final Graphics2D graphics2D = (Graphics2D) image.getGraphics();
        graphics2D.setColor(color);
        graphics2D.setStroke(new BasicStroke(thickness));
        graphics2D.draw(starShape);
    }

    public static void fill(BufferedImage image, Color color, int x0, int y0) {
        final int startRGB = image.getRGB(x0, y0);
        final int destRGB = color.getRGB();

        if (startRGB == destRGB) {
            return;
        }

        final Deque<Point> spansPoints = new ArrayDeque<>();
        final int imageWidth = image.getWidth();
        final int imageHeight = image.getHeight();

        final Point startSpanPoint = new Point(x0, y0);
        spansPoints.add(startSpanPoint);

        while (!spansPoints.isEmpty()) {
            final Point currentSpanPoint = spansPoints.pop();
            final int y = currentSpanPoint.y;

            int lx = currentSpanPoint.x;
            while (lx >= 0 && image.getRGB(lx, y) == startRGB) {
                image.setRGB(lx, y, destRGB);
                lx--;
            }

            int rx = currentSpanPoint.x + 1;
            while (rx < imageWidth && image.getRGB(rx, y) == startRGB) {
                image.setRGB(rx, y, destRGB);
                rx++;
            }

            if (y > 0) {
                scanForSpans(lx + 1, rx, y - 1, startRGB, image, spansPoints);
            }

            if (y < imageHeight - 1) {
                scanForSpans(lx + 1, rx, y + 1, startRGB, image, spansPoints);
            }
        }
    }

    private static void scanForSpans(int lx, int rx, int y, int startRGB, BufferedImage image, Deque<Point> spansPoints) {
        boolean foundSpan = false;
        int spanPointX = 0;

        for (int x = lx; x < rx; x++) {
            final int currentRGB = image.getRGB(x, y);

            if (!foundSpan && currentRGB == startRGB) {
                spanPointX = x;
                foundSpan = true;
            } else if (foundSpan && currentRGB != startRGB) {
                spansPoints.add(new Point(spanPointX, y));
                foundSpan = false;
            }
        }

        if (foundSpan) {
            spansPoints.add(new Point(spanPointX, y));
        }
    }

    private static void drawThinLine(BufferedImage image, Color color, int x1, int y1, int x2, int y2) {
        final int colorRGB = color.getRGB();
        final int dxAbs = Math.abs(x2 - x1);
        final int dyAbs = Math.abs(y2 - y1);

        if (dxAbs == 0) {
            final int minY = Integer.min(y1, y2);
            final int maxY = Integer.max(y1, y2);

            for (int y = minY; y <= maxY; y++) {
                image.setRGB(x1, y, colorRGB);
            }

            return;
        }

        if (dyAbs == 0) {
            final int minX = Integer.min(x1, x2);
            final int maxX = Integer.max(x1, x2);

            for (int x = minX; x <= maxX; x++) {
                image.setRGB(x, y1, colorRGB);
            }

            return;
        }

        final int dx = Integer.max(x1, x2) - Integer.min(x1, x2);
        final int dy = Integer.max(y1, y2) - Integer.min(y1, y2);
        final int delta = Integer.signum(y2 - y1) * Integer.signum(x2 - x1);

        if (dxAbs > dyAbs) {
            final int xOffset = Integer.min(x1, x2);
            int error = -dx;
            int y = x1 < x2 ? y1 : y2;

            for (int x = 0; x <= dx; x++) {
                error += 2 * dy;

                if (error > 0) {
                    error -= 2 * dx;
                    y += delta;
                }

                image.setRGB(x + xOffset, y, colorRGB);
            }
        } else {
            final int yOffset = Integer.min(y1, y2);
            int error = -dy;
            int x = y1 > y2 ? x2 : x1;

            for (int y = 0; y <= dy; y++) {
                error += 2 * dx;

                if (error > 0) {
                    error -= 2 * dy;
                    x += delta;
                }

                image.setRGB(x, y + yOffset, colorRGB);
            }
        }
    }

    private static Polygon createPolygon(int x0, int y0, int n, int radius, int rotationDegrees) {
        final double radians = Math.toRadians(rotationDegrees);
        final Polygon polygon = new Polygon();

        for (int i = 0; i < n; i++) {
            final double angle = i * 2 * Math.PI / n + radians;
            final int currentX = (int) Math.floor(x0 + radius * Math.cos(angle));
            final int currentY = (int) Math.floor(y0 + radius * Math.sin(angle));
            polygon.addPoint(currentX, currentY);
        }

        return polygon;
    }

    private static Shape createStarShape(double x0, double yo, double innerRadius, double outerRadius, int n, int rotationDegrees) {
        final Path2D path = new Path2D.Double();

        final double deltaAngle = Math.PI / n;
        final double radians = Math.toRadians(rotationDegrees);

        for (int i = 0; i < n * 2; i++) {
            final double angleRad = radians + i * deltaAngle;

            double relX = Math.cos(angleRad);
            double relY = Math.sin(angleRad);

            if (i % 2 == 0) {
                relX *= outerRadius;
                relY *= outerRadius;
            } else {
                relX *= innerRadius;
                relY *= innerRadius;
            }
            if (i == 0) {
                path.moveTo(x0 + relX, yo + relY);
            } else {
                path.lineTo(x0 + relX, yo + relY);
            }
        }

        path.closePath();

        return path;
    }
}
