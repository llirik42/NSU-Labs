package ru.nsu.kondrenko.model.image;

import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;

public class ImageReaderImpl implements ImageReader {
    private static final String[] SUPPORTED_FORMATS = {"gif", "png", "jpeg", "jpg", "jpeg", "bmp", "wbmp"};

    @Override
    public BufferedImage read(File file) throws IOException {
        try {
            return ImageIO.read(file);
        } catch (IOException exception) {
            throw new IOException(String.format("Error during reading file \"%s\"", file.getAbsolutePath()), exception);
        }
    }

    @Override
    public BufferedImage readAsResource(String resourceName) throws IOException {
        try (InputStream inputStream = this.getClass().getResourceAsStream(resourceName)) {
            if (inputStream == null) {
                throw new IOException(String.format("Resource \"%s\" not found", resourceName));
            }

            return ImageIO.read(inputStream);
        } catch (IOException exception) {
            throw new IOException(String.format("Error during reading resource \"%s\"", resourceName), exception);
        }
    }

    @Override
    public String[] getSupportedFormats() {
        return SUPPORTED_FORMATS.clone();
    }
}
