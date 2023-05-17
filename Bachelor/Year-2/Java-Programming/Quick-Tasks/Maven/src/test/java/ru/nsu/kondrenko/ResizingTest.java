package ru.nsu.kondrenko;

import org.imgscalr.Scalr;
import org.junit.jupiter.api.Test;

import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.InputStream;

import static org.junit.jupiter.api.Assertions.*;

class ResizingTest {
    @Test
    public void increaseSizeTest() {
        resizingTest(2);
    }

    @Test
    public void decreaseSizeTest() {
        resizingTest(0.5);
    }

    private void resizingTest(double coefficient) {
        try (InputStream inputStream = Main.class.getResourceAsStream("/image.jpg")) {
            assert inputStream != null;

            BufferedImage oldImage = ImageIO.read(inputStream);

            final int newWidth = (int) (oldImage.getWidth() * coefficient);
            final int newHeight = (int) (oldImage.getHeight() * coefficient);

            BufferedImage newImage = Scalr.resize(oldImage, newWidth, newHeight);

            assertEquals(newWidth, newImage.getWidth());
            assertEquals(newHeight, newImage.getHeight());
        } catch (IOException exception) {
            throw new RuntimeException(exception);
        }
    }
}
