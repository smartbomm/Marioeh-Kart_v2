import java.util.ArrayList;
import java.util.List;

import javax.swing.JFrame;


import de.marioehkart.marioehkarte.*;

public class App {
    public static void main(String[] args) throws Exception {
        Map marioehKarte = new Map("./test");
        try {
            marioehKarte.feed("./Bahn Abschnitt 1.csv", ";");
        } catch (Exception e) {
            e.printStackTrace();
        }
        Coordinates coordinates = marioehKarte.getCoordinates(1, 367);
        System.out.println("Coordinates: " + coordinates.x + ", " + coordinates.y + ", " + coordinates.angle_x);
        coordinates = marioehKarte.getCoordinates(2, 367);
        System.out.println("Coordinates: " + coordinates.x + ", " + coordinates.y + ", " + coordinates.angle_x);
        coordinates = marioehKarte.getCoordinates(3, 367);
        System.out.println("Coordinates: " + coordinates.x + ", " + coordinates.y + ", " + coordinates.angle_x);

        marioehKarte.close();
    }
}
