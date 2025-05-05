public class Coordinates {
    public Coordinates() {
        this.x = 0;
        this.y = 0;
        this.angle_x = 0;
    }
    public double x;
    public double y;
    public double angle_x;
    public static Coordinates addCoordinates(Coordinates a, Coordinates b) {
        Coordinates result = new Coordinates();
        result.x = a.x + b.x;
        result.y = a.y + b.y;
        result.angle_x = a.angle_x + b.angle_x;
        return result;
    }
    public static Coordinates transformCoordinates(Coordinates a, double angle) {
        Coordinates result = new Coordinates();
        result.x = a.x * Math.cos(angle) - a.y * Math.sin(angle);
        result.y = a.x * Math.sin(angle) + a.y * Math.cos(angle);
        result.angle_x = a.angle_x + angle;
        return result;
    }
}
