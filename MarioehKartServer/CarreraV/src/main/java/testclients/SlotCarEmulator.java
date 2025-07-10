package testclients;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;

public class SlotCarEmulator {
    
    // Simulationsparameter
    static final double dt = 0.1;                  // Zeitschritt in Sekunden
    static final double engineAcceleration = 2.0;  // Beschleunigung in m/s²
    static final double brakingDeceleration = -4.0; // Verzögerung in m/s² (negativ)
    
    // UDP‑Konfiguration
    static final String SERVER_IP = "127.0.0.1";
    static final int UDP_PORT = 4210;
    static final String carId = "Car1";
    // Beispielhafte MAC-Adresse (wird in der Nachricht genutzt)
    static final String MAC_ADDRESS = "00:11:22:33:44:55";
    
    public static void main(String[] args) {
        try (DatagramSocket socket = new DatagramSocket()) {
            InetAddress serverAddress = InetAddress.getByName(SERVER_IP);
            
            // Definiere die Bahn als Liste von TrackPoints.
            // Jeder Punkt enthält x, y und einen sicheren (maximalen) Speed in m/s, der z. B. aus der Kurvengeometrie abgeleitet wurde.
            List<TrackPoint> trackPoints = new ArrayList<>();
            trackPoints.add(new TrackPoint(0, 0, 12));     // Gerade Strecke, höhere Geschwindigkeit möglich
            trackPoints.add(new TrackPoint(50, 0, 12));
            trackPoints.add(new TrackPoint(100, 50, 6));     // Kurve, geringerer safe speed
            trackPoints.add(new TrackPoint(100, 100, 6));
            trackPoints.add(new TrackPoint(50, 150, 12));
            trackPoints.add(new TrackPoint(0, 150, 12));
            trackPoints.add(new TrackPoint(-50, 100, 8));    // Leichtere Kurve
            // Erstelle die Bahn (geschlossen)
            Track track = new Track(trackPoints);
            
            // Fahrzeugzustand
            double s = 0;           // Zurückgelegte Strecke entlang der Bahn (in Metern)
            double v = 0;           // Geschwindigkeit in m/s
            double a = 0;           // Tangentiale Beschleunigung in m/s²
            double simulationTime = 0;  // Simulationszeit in Sekunden
            
            while (true) {
                // Ermittle aktuellen Abschnitt und safe speed anhand der aktuellen Strecke s
                int section = track.getSectionIndex(s);
                double safeSpeed = track.getSafeSpeed(s);
                
                // Simpler Regler: Unterhalb des safe speed beschleunigen, oberhalb bremsen
                if (v < safeSpeed) {
                    a = engineAcceleration;
                } else {
                    a = brakingDeceleration;
                }
                
                // Aktualisiere Geschwindigkeit und Position (Euler‑Integration)
                v = v + a * dt;
                if (v < 0) v = 0; // Keine Rückwärtsfahrt
                s = s + v * dt;
                // Bei Überschreitung der Streckenlänge wird geschlossen fortgesetzt
                s = s % track.getTotalLength();
                
                // Berechne die aktuelle (x,y)-Position auf der Bahn
                double[] pos = track.getPosition(s);
                // Ermittle die Krümmung des aktuellen Abschnitts als Näherung.
                double curvature = track.getCurvature(section);
                // Lateralbeschleunigung (Zentripetalbeschleunigung) ≈ v² * curvature
                double lateralAcc = v * v * curvature;
                
                // Erstelle den Nachrichtentext, der die Sensorwerte simuliert.
                // Das Format ist CSV-artig und enthält:
                // MAC-Adresse, t_section (aktueller Streckenabschnitt),
                // pos/x, pos/y, pos/l (Strecke entlang der Bahn),
                // speed/l (Geschwindigkeit),
                // accell/x (tangentiale Beschleunigung), accell/y (laterale Beschleunigung), accell/z (konstant 0),
                // time (Simulationszeit in Millisekunden)
                long simTimeMs = (long)(simulationTime * 1000);
                String message = MAC_ADDRESS + ","
                        + "t_section=" + section + ","
                        + "pos/x=" + String.format("%.2f", pos[0]) + ","
                        + "pos/y=" + String.format("%.2f", pos[1]) + ","
                        + "pos/l=" + String.format("%.2f", s) + ","
                        + "speed/l=" + String.format("%.2f", v) + ","
                        + "accell/x=" + String.format("%.2f", a) + ","
                        + "accell/y=" + String.format("%.2f", lateralAcc) + ","
                        + "accell/z=0" + ","
                        + "time=" + simTimeMs;
                
                // Sende das UDP-Paket
                byte[] buffer = message.getBytes(StandardCharsets.UTF_8);
                DatagramPacket packet = new DatagramPacket(buffer, buffer.length, serverAddress, UDP_PORT);
                socket.send(packet);
                
                System.out.println("Gesendet: " + message);
                
                // Aktualisiere die Simulationszeit und warte bis zum nächsten Zeitschritt
                simulationTime += dt;
                Thread.sleep((long)(dt * 1000));
            }
            
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    // ------------------- Hilfsklassen -------------------
    
    // Repräsentiert einen Punkt auf der Bahn inklusive eines zulässigen Geschwindigkeitswertes (safe speed)
    static class TrackPoint {
        double x, y;
        double safeSpeed; // in m/s
        
        TrackPoint(double x, double y, double safeSpeed) {
            this.x = x;
            this.y = y;
            this.safeSpeed = safeSpeed;
        }
    }
    
    // Die Track-Klasse kapselt eine geschlossene Bahn, die über eine Liste von TrackPoints definiert ist.
    // Sie berechnet Segmentlängen, ermöglicht die Interpolation der Position und ermittelt den sicheren
    // Geschwindigkeitswert (safe speed) sowie eine Näherung für die Krümmung.
    static class Track {
        List<TrackPoint> points;
        List<Double> segmentLengths;
        double totalLength;
        
        public Track(List<TrackPoint> points) {
            this.points = points;
            computeSegmentLengths();
        }
        
        // Berechnet die Länge jedes Streckenabschnitts und die Gesamtlänge der Bahn.
        private void computeSegmentLengths() {
            segmentLengths = new ArrayList<>();
            totalLength = 0;
            for (int i = 0; i < points.size(); i++) {
                TrackPoint p1 = points.get(i);
                TrackPoint p2 = points.get((i+1) % points.size());
                double dx = p2.x - p1.x;
                double dy = p2.y - p1.y;
                double len = Math.sqrt(dx*dx + dy*dy);
                segmentLengths.add(len);
                totalLength += len;
            }
        }
        
        public double getTotalLength() {
            return totalLength;
        }
        
        // Gibt den Index des aktuellen Segments zurück, in dem die Strecke s liegt.
        public int getSectionIndex(double s) {
            double distance = s;
            for (int i = 0; i < segmentLengths.size(); i++) {
                if (distance <= segmentLengths.get(i)) {
                    return i;
                }
                distance -= segmentLengths.get(i);
            }
            return segmentLengths.size()-1;
        }
        
        // Bestimmt den sicheren Geschwindigkeitswert (safe speed) an der Stelle s der Bahn durch lineare Interpolation.
        public double getSafeSpeed(double s) {
            double distance = s;
            int segIndex = getSectionIndex(s);
            double segLength = segmentLengths.get(segIndex);
            double fraction = distance / segLength;
            TrackPoint p1 = points.get(segIndex);
            TrackPoint p2 = points.get((segIndex+1) % points.size());
            return p1.safeSpeed + fraction * (p2.safeSpeed - p1.safeSpeed);
        }
        
        // Bestimmt die (x,y)-Position auf der Bahn an der Strecke s (lineare Interpolation zwischen zwei Punkten).
        public double[] getPosition(double s) {
            double distance = s;
            int segIndex = getSectionIndex(s);
            double segLength = segmentLengths.get(segIndex);
            double fraction = distance / segLength;
            TrackPoint p1 = points.get(segIndex);
            TrackPoint p2 = points.get((segIndex+1) % points.size());
            double x = p1.x + fraction * (p2.x - p1.x);
            double y = p1.y + fraction * (p2.y - p1.y);
            return new double[]{x, y};
        }
        
        // Näherung der Krümmung am Übergang eines Segments.
        // Hier wird der Winkelunterschied zwischen dem aktuellen Segment und dem nächsten relativ zur Segmentlänge herangezogen.
        public double getCurvature(int segIndex) {
            TrackPoint p0 = points.get((segIndex - 1 + points.size()) % points.size());
            TrackPoint p1 = points.get(segIndex);
            TrackPoint p2 = points.get((segIndex+1) % points.size());
            
            double angle1 = Math.atan2(p1.y - p0.y, p1.x - p0.x);
            double angle2 = Math.atan2(p2.y - p1.y, p2.x - p1.x);
            double dTheta = angle2 - angle1;
            // Normalisiere den Winkel auf [-π, π]
            while (dTheta > Math.PI) dTheta -= 2 * Math.PI;
            while (dTheta < -Math.PI) dTheta += 2 * Math.PI;
            
            double segLength = segmentLengths.get(segIndex);
            return Math.abs(dTheta) / segLength;
        }
    }
}
