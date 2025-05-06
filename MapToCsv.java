import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

import de.marioehkart.marioehkarte.*;

public class MapToCsv {

    public static void main(String[] args) throws Exception {
        String originCsv = args[0]; // Pfad zur Quell-Datei
        String destination = args[1]; // Pfad zur Zieldatei
        int occuracy = Integer.parseInt(args[2]); // Genauigkeit

        Coordinates sectionCoordinates = new Coordinates();
        Section section = null;

        BufferedReader br = new BufferedReader(new FileReader(originCsv));
        FileWriter writer = null;

        String zeile;
        zeile = br.readLine(); // erste Zeile überspringen
        while ((zeile = br.readLine()) != null) {
            String[] werte = zeile.split(";"); // einfache Trennung mit Komma
            System.out.print(werte[0]);
            if (werte[0].equals("SECTION_START")) {
                System.out.print(": " + werte[1]);
                if(writer != null) {
                    writer.close();
                    writer = null;
                }
                writer = new FileWriter(destination + "/" + werte[1] + ".csv");
                sectionCoordinates.x = Double.parseDouble(werte[3]);
                sectionCoordinates.y = Double.parseDouble(werte[4]);
                sectionCoordinates.angle_x = Math.toRadians(Double.parseDouble(werte[5]));
                section = new Section(sectionCoordinates, Integer.parseInt(werte[2]));
            } else if (werte[0].equals("SECTION_END")){
                for (int i = 0; i < section.length(); i += occuracy) {
                    Coordinates c = new Coordinates();
                    c = section.position(i);
                    Coordinates.addCoordinates(sectionCoordinates, c);
                    String coordinatesToCsv = c.x + "," + c.y + "," + c.angle_x + "\n";
                    writer.append(coordinatesToCsv);
                }
                writer.close();
            } else {
                section.addRail(Rail.fromString(werte[0]));
            }
            System.out.println();
        }
        writer.close();
        br.close();
    }
}
