import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.List;
import java.util.ArrayList;

public class CoordinatePlotSwing extends JPanel {

    private final List<Coordinates> coordinates;
    private double scale = 40.0;

    public CoordinatePlotSwing(List<Coordinates> coordinates) {
        this.coordinates = coordinates;
        setPreferredSize(new Dimension(800, 800));
        setBackground(Color.WHITE);

        // Zoom mit Mausrad
        addMouseWheelListener(e -> {
            double delta = e.getPreciseWheelRotation();
            scale *= (1 - delta * 0.1);
            scale = 1; // Begrenzung
            repaint();
        });
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        drawCoordinates((Graphics2D) g);
    }

    private void drawCoordinates(Graphics2D g) {
        int w = getWidth();
        int h = getHeight();
        int centerX = w / 2;
        int centerY = h / 2;

        g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

        // Gitternetz
        g.setColor(new Color(230, 230, 230));
        for (int x = centerX % (int) scale; x < w; x += scale) {
            g.drawLine(x, 0, x, h);
        }
        for (int y = centerY % (int) scale; y < h; y += scale) {
            g.drawLine(0, y, w, y);
        }

        // Achsen
        g.setColor(Color.GRAY);
        g.setStroke(new BasicStroke(2));
        g.drawLine(0, centerY, w, centerY); // X-Achse
        g.drawLine(centerX, 0, centerX, h); // Y-Achse

        // Punkte und Richtung
        g.setColor(Color.BLUE);
        int index = 1;
        for (Coordinates c : coordinates) {
            int x = (int) (centerX + c.x * scale);
            int y = (int) (centerY - c.y * scale);

            g.fillOval(x - 4, y - 4, 8, 8);

            // Richtungslinie
            double angleRad = Math.toRadians(c.angle_x);
            int dx = (int) (Math.cos(angleRad) * 20);
            int dy = (int) (-Math.sin(angleRad) * 20); // Y-Achse invertiert

            g.drawLine(x, y, x + dx, y + dy);

            // Beschriftung
            g.setColor(Color.BLACK);
            g.drawString("P" + index++, x + 6, y - 6);
            g.setColor(Color.BLUE);
        }
    }

    public static void main(String[] args) {
        List<Coordinates> list = new ArrayList<>();

        Coordinates c1 = new Coordinates(); // (0,0,0)
        Coordinates c2 = new Coordinates();
        c2.x = 2;
        c2.y = 1;
        c2.angle_x = 45;

        Coordinates c3 = new Coordinates();
        c3.x = -3;
        c3.y = -2;
        c3.angle_x = 135;

        list.add(c1);
        list.add(c2);
        list.add(c3);
        list.add(Coordinates.addCoordinates(c2, c3)); // Beispiel für addCoordinates

        JFrame frame = new JFrame("Koordinaten-Plotter (Swing)");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setContentPane(new CoordinatePlotSwing(list));
        frame.pack();
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
    }
}
