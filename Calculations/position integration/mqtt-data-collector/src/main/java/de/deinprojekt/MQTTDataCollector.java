package de.deinprojekt;
import org.eclipse.paho.client.mqttv3.*;
import java.io.*;
import java.util.*;
import com.opencsv.CSVWriter;
import org.apache.poi.ss.usermodel.*;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;
public class MQTTDataCollector {
    static final String broker = "tcp://192.168.9.150:1883";
    static final String clientId = "JavaClient";
    static final String macAddress = "C/A4:69:81:1F:B6:08";
    static final int readTime = 20;
//IP von Server 192.168.9.150:1883
//A0:71:81:1F:B6:08 Prototyp Julian
//D4:7C:4C:1C:B2:B0 Prototyp Panzertape
//A4:69:81:1F:B6:08 Unser Prototyp

    static List<String> accelX = Collections.synchronizedList(new ArrayList<>());
    static List<String> accelY = Collections.synchronizedList(new ArrayList<>());
    static List<String> accelZ = Collections.synchronizedList(new ArrayList<>());
    static List<String> timeList = Collections.synchronizedList(new ArrayList<>());
    static List<String> gyroX = Collections.synchronizedList(new ArrayList<>());
    static List<String> gyroY = Collections.synchronizedList(new ArrayList<>());
    static List<String> gyroZ = Collections.synchronizedList(new ArrayList<>());
    static List<String> accelL = Collections.synchronizedList(new ArrayList<>());
    static List<String> speedL = Collections.synchronizedList(new ArrayList<>());
    static List<String> posL = Collections.synchronizedList(new ArrayList<>());
    static List<String> tracks = Collections.synchronizedList(new ArrayList<>());

    public static void main(String[] args) throws Exception {
        MqttClient client = new MqttClient(broker, clientId);
        client.connect();

        System.out.println("Verbindung hergestellt.");

        // Topics abonnieren + Listener registrieren
        subscribe(client, macAddress + "/t", timeList);
        subscribe(client, macAddress + "/a/x", accelX);
        subscribe(client, macAddress + "/a/y", accelY);
        subscribe(client, macAddress + "/a/z", accelZ);
        subscribe(client, macAddress + "/g/x", gyroX);
        subscribe(client, macAddress + "/g/y", gyroY);
        subscribe(client, macAddress + "/g/z", gyroZ);
        subscribe(client, macAddress + "/p/l", posL);
        subscribe(client, macAddress + "/s/l", speedL);
        subscribe(client, macAddress + "/a/l", accelL);
        subscribe(client, macAddress + "/ts", tracks);

        System.out.println("Losfahren...");

        // Datensammelzeit: z. B. 20 Sekunden
        Thread.sleep(readTime * 1000);

        System.out.println("Time:   " + timeList.size());
        System.out.println("AccelX: " + accelX.size());
        System.out.println("AccelY: " + accelY.size());
        System.out.println("AccelZ: " + accelZ.size());
        System.out.println("GyroX:  " + gyroX.size());
        System.out.println("GyroY:  " + gyroY.size());
        System.out.println("GyroZ:  " + gyroZ.size());
        System.out.println("AccelL: " + accelL.size());
        System.out.println("SpeedL: " + speedL.size());
        System.out.println("PosL:   " + posL.size());
        System.out.println("TrackSec: " + tracks.size());

        client.disconnect();
        client.close();

        // Schreiben in Excel
        writeExcel("Section_5_C_Code_Buffersize30_debug_integrationonmc.xlsx");
    }

    private static void subscribe(MqttClient client, String topic, List<String> targetList) throws MqttException {
        client.subscribe(topic, (t, msg) -> {
            String payload = new String(msg.getPayload());
            targetList.add(payload);
        });
    }

    private static void writeExcel(String filename) throws IOException {
        Workbook wb = new XSSFWorkbook();
        writeSheet(wb, "Time", timeList);
        writeSheet(wb, "AccelX", accelX);
        writeSheet(wb, "AccelY", accelY);
        writeSheet(wb, "AccelZ", accelZ);
        writeSheet(wb, "GyroX", gyroX);
        writeSheet(wb, "GyroY", gyroY);
        writeSheet(wb, "GyroZ", gyroZ);
        writeSheet(wb, "AccelL", accelL);
        writeSheet(wb, "SpeedL", speedL);
        writeSheet(wb, "PosL", posL);
        writeSheet(wb, "TrackSec",tracks);
        

        String outputPath = "C:/Users/gesel/OneDrive/Dokumente/MATLAB/" + filename;
        try (FileOutputStream out = new FileOutputStream(outputPath)) {

            wb.write(out);
        }
        wb.close();
        System.out.println("Excel-Datei geschrieben: " + filename);
    }

    private static void writeSheet(Workbook wb, String name, List<String> data) {
        Sheet sheet = wb.createSheet(name);
        for (int i = 0; i < data.size(); i++) {
            Row row = sheet.createRow(i);
            row.createCell(0).setCellValue(data.get(i));
        }
    }
}



