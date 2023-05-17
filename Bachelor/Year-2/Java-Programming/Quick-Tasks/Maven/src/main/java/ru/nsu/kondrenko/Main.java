package ru.nsu.kondrenko;

import org.imgscalr.Scalr;

import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.Scanner;

public class Main {
    private static final Scanner scanner = new Scanner(System.in);

    public static void main(String[] args) {
        try (InputStream inputStream = Main.class.getResourceAsStream("/image.jpg")) {
            if (inputStream == null) {
                System.err.println("Image not found!");
                System.exit(-1);
            }

            int newSize = inputNumber("size");

            BufferedImage oldImage = ImageIO.read(inputStream);

            BufferedImage newImage = Scalr.resize(oldImage, newSize, newSize);

            File outputfile = new File("./new_image.png");

            ImageIO.write(newImage, "png", outputfile);

            System.out.println("Image resized!!!");
        } catch (IOException exception) {
            throw new RuntimeException(exception);
        }
    }

    private static int inputNumber(String name) {
        System.out.print(name + ": ");
        return scanner.nextInt();
    }
}
