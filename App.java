import java.util.ArrayList;
import java.util.List;

import javax.swing.JFrame;

public class App {
    public static void main(String[] args) {
        // Hier können Sie Ihre Logik implementieren
        System.out.println("Hallo, Welt!");

        Rail gerade_standard = new Rail(345, 0);
        Rail kurve_R1_innen = new Rail(129.6, 247.5);
        Rail kurve_R1_aussen = new Rail(181.4, 346.5);
        Rail kurve_R1_aussen_ = new Rail(181.4, -346.5);


        Section s_2 = new Section(new Coordinates(), 12);
        for (int i = 0; i < 12; i++) {
            s_2.addRail(kurve_R1_innen);
        }
        System.out.println("Länge der Sektion: " + s_2.length());

        List<Coordinates> list = new ArrayList<>();

        for (int i = 0; i < s_2.length(); i+=10) {
            Coordinates c = new Coordinates();
            c = s_2.position(i);
            list.add(c);
        }

        JFrame frame = new JFrame("Koordinaten-Plotter (Swing)");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setContentPane(new CoordinatePlotSwing(list));
        frame.pack();
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
    }
}
