package MarioConverter;

import de.marioehkart.marioehkarte.Map;
import de.marioehkart.marioehkarte.Coordinates;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;

import javax.swing.*;
import java.awt.Color;
import java.io.FileWriter;

public class writecsvoutoftrack {

    public static void main(String[] args) throws Exception {
        // Load map data
        Map carreraMap = new Map("logs");
        carreraMap.feed("BahnKarte.csv", ";");

        XYSeriesCollection dataset = new XYSeriesCollection();
        StringBuilder csvOutput = new StringBuilder();
        csvOutput.append("Section,Pos,Y,X\n");  // CSV header

        for (int fixedSection = 0; fixedSection <= 45; fixedSection++) {
            XYSeries series = new XYSeries("Section " + fixedSection);
            int pos = 0;

            while (true) {
                try {
                    Coordinates coords = carreraMap.getCoordinates(fixedSection, pos);
                    series.add(-coords.y, coords.x);

                    // Add to CSV
                    csvOutput.append(fixedSection).append(",")
                             .append(pos).append(",")
                             .append(-coords.y).append(",")
                             .append(coords.x).append("\n");

                    pos++;
                } catch (Exception e) {
                    if (pos == 0) {
                        System.err.println("Skipping invalid section " + fixedSection + ": " + e.getMessage());
                    } else {
                        System.err.println("Section " + fixedSection + " ended at pos_lin=" + pos + ": " + e.getMessage());
                        dataset.addSeries(series);  // Add only valid data
                    }
                    break;
                }
            }
        }

        // Write CSV to file
        try (FileWriter writer = new FileWriter("TrackCoordinatesOutput.csv")) {
            writer.write(csvOutput.toString());
            System.out.println("CSV export completed: TrackCoordinatesOutput.csv");
        } catch (Exception e) {
            System.err.println("Error writing CSV: " + e.getMessage());
        }

        // Create chart
        JFreeChart chart = ChartFactory.createScatterPlot(
                "Track Sections Plot",
                "X",
                "Y",
                dataset,
                PlotOrientation.VERTICAL,
                true,
                true,
                false
        );

        chart.getPlot().setBackgroundPaint(Color.white);

        // Display chart
        JFrame frame = new JFrame("Marioe Track Plot");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.add(new ChartPanel(chart));
        frame.setSize(1800, 650);
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
    }
}
