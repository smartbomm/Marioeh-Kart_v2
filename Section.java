

public class Section {

    private class _Rail {
        Rail rail;
        double angle;
    }

    private Coordinates startPosition;
    private _Rail rails[];
    private int railCount;
    private int length;
    private double angle;

    private Coordinates railCoordinates (_Rail rail, double distance) {
        Coordinates result = new Coordinates();
        result = Coordinates.transformCoordinates(rail.rail.coordinates(distance), rail.angle);
        return result;
    }
    
    public Section(Coordinates start, int amount) {
        this.startPosition = start;
        this.railCount = 0;
        this.rails = new _Rail[amount];
        this.length = 0;
        this.angle = 0;
        for (int i = 0; i < amount; i++) {
            this.rails[i] = new _Rail();
        }
    }
    public void addRail(Rail rail) {
        if (railCount >= rails.length) {
            throw new IllegalArgumentException("Rail array is full");
        } else {
            rails [railCount].rail = rail;
            rails[railCount].angle = this.angle;
            railCount++;
            length += rail.length();
            this.angle += rail.coordinates(rail.length()).angle_x;
        }
    }
    public Coordinates position (double distance) {
        if (distance > length) {
            throw new IllegalArgumentException("Distance exceeds section length");
        }
        Coordinates result = new Coordinates();
        result = Coordinates.addCoordinates(result, startPosition);
        for (int i = 0; i< railCount; i++) {
            if (distance > rails[i].rail.length()) {
                distance -= rails[i].rail.length();
                result = Coordinates.addCoordinates(result, railCoordinates(rails[i], rails[i].rail.length()));
            } else {
                result = Coordinates.addCoordinates(result, railCoordinates(rails[i], distance));
                return result;
            }
        }

        throw new IllegalArgumentException("No rail found for the given distance");
    }
    public double length() {
        return this.length;
    }


}
