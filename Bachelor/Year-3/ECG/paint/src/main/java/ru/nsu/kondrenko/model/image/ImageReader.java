package ru.nsu.kondrenko.model.image;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

public interface ImageReader {
    BufferedImage read(File file) throws IOException;

    BufferedImage readAsResource(String resourceName) throws IOException;

    String[] getSupportedFormats();
}
