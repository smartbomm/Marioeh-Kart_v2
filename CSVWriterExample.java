import java.io.FileWriter;
import java.io.IOException;

public class CSVWriterExample {

    public static void main(String[] args) {
        String csvFile = "beispiel.csv";

        try (FileWriter writer = new FileWriter(csvFile)) {
            // Kopfzeile
            writer.append("Name,Alter,Stadt\n");

            // Datenzeilen
            writer.append("Anna,30,Berlin\n");
            writer.append("Ben,25,Hamburg\n");
            writer.append("Clara,28,München\n");

            System.out.println("CSV-Datei erfolgreich geschrieben: " + csvFile);
        } catch (IOException e) {
            System.out.println("Fehler beim Schreiben der CSV-Datei.");
            e.printStackTrace();
        }
    }
}
