package ru.nsu.kondrenko.model.image;

import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

public class ImageSaverImpl implements ImageSaver {
    @Override
    public void saveAsPNG(BufferedImage image, File file) throws IOException {
        try {
            ImageIO.write(image, "png", file);
        } catch (IOException exception) {
            throw new IOException(String.format("Error while saving image to \"%s\"", file.getAbsolutePath()));
        }
    }
}
