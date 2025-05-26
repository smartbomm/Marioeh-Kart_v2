import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.List;
import java.util.ArrayList;
import javax.swing.event.*;
import java.awt.geom.Line2D;


class ZoomSlider extends JPanel {
    public ZoomSlider(CoordinateVisualizer visualizer) {
        setLayout(new BorderLayout());
        JLabel label = new JLabel("Zoom:");
        JSlider slider = new JSlider(0, 100, 50); // Mittelstellung = 1.0 Zoom

        slider.addChangeListener(e -> {
            int val = slider.getValue();
            double scale = Math.pow(2, (val - 50) / 10.0); // Logarithmische Skalierung
            visualizer.setZoom(scale);
        });

        add(label, BorderLayout.WEST);
        add(slider, BorderLayout.CENTER);
    }
}

public class CoordinateVisualizer extends JPanel {

    private List<DataPoint> dataPoints;
    private double zoom = 1.0;
    private double offsetX = 0.0, offsetY = 0.0;
    private double maxX = 0, maxY = 0;
    private double minX = 0, minY = 0;
    private boolean dragging = false;
    private Point dragStart;

    public CoordinateVisualizer(String filePath) {
        dataPoints = loadCSV(filePath);
        setPreferredSize(new Dimension(800, 800));
        addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent e) {
                if (e.getButton() == MouseEvent.BUTTON1) {
                    dragging = true;
                    dragStart = e.getPoint();
                }
            }

            @Override
            public void mouseReleased(MouseEvent e) {
                dragging = false;
            }
        });

        addMouseMotionListener(new MouseAdapter() {
            @Override
            public void mouseDragged(MouseEvent e) {
                if (dragging) {
                    offsetX += (e.getX() - dragStart.x) / zoom;
                    offsetY += (e.getY() - dragStart.y) / zoom;
                    dragStart = e.getPoint();
                    repaint();
                }
            }
        });

        addMouseWheelListener(e -> {
            double delta = e.getWheelRotation();
            zoom *= (delta < 0) ? 1.1 : 0.9;
            repaint();
        });
    }

    private List<DataPoint> loadCSV(String filePath) {
        List<DataPoint> points = new ArrayList<>();
        try (BufferedReader reader = new BufferedReader(new FileReader(filePath))) {
            String line;
            while ((line = reader.readLine()) != null) {
                String[] values = line.split(",");
                if (values.length == 3) {
                    double x = Double.parseDouble(values[0].trim());
                    double y = Double.parseDouble(values[1].trim());
                    double angle = Double.parseDouble(values[2].trim());
                    points.add(new DataPoint(x, y, angle));
                    maxX = Math.max(maxX, x);
                    minX = Math.min(minX, x);
                    maxY = Math.max(maxY, y);
                    minY = Math.min(minY, y);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return points;
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        Graphics2D g2d = (Graphics2D) g;
        g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        
        // Background and grid
        g2d.setColor(Color.LIGHT_GRAY);
        g2d.fillRect(0, 0, getWidth(), getHeight());
        drawGrid(g2d);
        
        // Draw points and their angles
        g2d.setColor(Color.BLACK);
        for (DataPoint point : dataPoints) {
            double x = (point.getX() - minX) * zoom + offsetX;
            double y = (point.getY() - minY) * zoom + offsetY;
            g2d.fillOval((int) x - 2, (int) y - 2, 4, 4);
            drawAngle(g2d, point, x, y);
        }
    }

    private void drawGrid(Graphics2D g2d) {
        g2d.setColor(Color.GRAY);
        int step = 20;
        for (int i = 0; i < getWidth(); i += step) {
            g2d.drawLine(i, 0, i, getHeight());
            g2d.drawLine(0, i, getWidth(), i);
        }

        g2d.setColor(Color.BLACK);
        g2d.drawLine(getWidth() / 2, 0, getWidth() / 2, getHeight());
        g2d.drawLine(0, getHeight() / 2, getWidth(), getHeight() / 2);
    }

    private void drawAngle(Graphics2D g2d, DataPoint point, double x, double y) {
        double angle = point.getAngle();
        double length = 20; // Length of the arrow
        double dx = Math.cos(angle) * length;
        double dy = Math.sin(angle) * length;
        g2d.draw(new Line2D.Double(x, y, x + dx, y - dy));
    }

    public static class DataPoint {
        private double x, y, angle;

        public DataPoint(double x, double y, double angle) {
            this.x = x;
            this.y = y;
            this.angle = angle;
        }

        public double getX() {
            return x;
        }

        public double getY() {
            return y;
        }

        public double getAngle() {
            return angle;
        }
    }

    

    public void setZoom(double zoom) {
        this.zoom = zoom;
        repaint();
    }
    

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            JFrame frame = new JFrame("Coordinate Visualizer");
            String filePath = "/Users/bmmli.jl/wolke7/Hochschule/25SS/Projekt EPE-CPS/carrera-map/out/3.csv";  // Set your CSV file path here
    
            CoordinateVisualizer panel = new CoordinateVisualizer(filePath);
            ZoomSlider zoomSlider = new ZoomSlider(panel);
    
            JPanel mainPanel = new JPanel(new BorderLayout());
            mainPanel.add(panel, BorderLayout.CENTER);
            mainPanel.add(zoomSlider, BorderLayout.SOUTH);
    
            frame.add(mainPanel);
            frame.setSize(900, 900);
            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            frame.setVisible(true);
        });
    }
    
    
}

