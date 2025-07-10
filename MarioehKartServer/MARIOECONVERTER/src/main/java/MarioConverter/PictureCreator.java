package MarioConverter;

import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import javax.imageio.ImageIO;

public class PictureCreator {
    public static void main(String[] args) {
        String inputPath = "TrackCoordinatesOutput.csv";
        String outputPath = "background.png";

        List<Double> xs = new ArrayList<>();
        List<Double> ys = new ArrayList<>();

        // 1) Read CSV and extract swapped coordinates coordinate system is set the other way arround so everything is positive
        try {
            List<String> lines = Files.readAllLines(Paths.get(inputPath));
            for (String line : lines.subList(1, lines.size())) { // skip header
                String[] cols = line.split(",");
                if (cols.length < 4) continue;
                try {
                    double x = Double.parseDouble(cols[2]); // col[2] -> X
                    double y = Double.parseDouble(cols[3]); // col[3] -> Y
                    xs.add(x);
                    ys.add(y);
                } catch (NumberFormatException e) {
                    // Skip invalid entries
                }
            }
        } catch (IOException e) {
            System.err.println("Failed to read CSV file: " + e.getMessage());
            return;
        }

        if (xs.isEmpty() || ys.isEmpty()) {
            System.out.println("No valid data points found.");
            return;
        }

        // 2) Compute bounds
        double minX = xs.stream().min(Double::compare).orElse(0.0);
        double maxX = xs.stream().max(Double::compare).orElse(1.0);
        double minY = ys.stream().min(Double::compare).orElse(0.0);
        double maxY = ys.stream().max(Double::compare).orElse(1.0);
        double rangeX = maxX - minX;
        double rangeY = maxY - minY;

        if (rangeX == 0) rangeX = 1;
        if (rangeY == 0) rangeY = 1;

        // 3) Scale
        double scale = 1000.0 / Math.max(rangeX, rangeY);

        // 4) Image size
        int width = (int) (rangeX * scale) + 1;
        int height = (int) (rangeY * scale) + 1;
        System.out.printf("Creating image of size %d×%d (scale=%.4f px/unit)%n", width, height, scale);

        // 5) Create image and plot
        BufferedImage image = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                image.setRGB(x, y, Color.WHITE.getRGB());
            }
        }

        for (int i = 0; i < xs.size(); i++) {
            int sx = (int) ((xs.get(i) - minX) * scale);
            int sy = (int) ((ys.get(i) - minY) * scale);
            sy = height - 1 - sy; // flip vertically
            if (sx >= 0 && sx < width && sy >= 0 && sy < height) {
                image.setRGB(sx, sy, new Color(128, 128, 128).getRGB());
            }
        }

        // 6) Save image
        try {
            ImageIO.write(image, "png", Paths.get(outputPath).toFile());
            System.out.println("Saved scaled image as " + outputPath);
        } catch (IOException e) {
            System.err.println("Failed to save image: " + e.getMessage());
        }
    }
}
