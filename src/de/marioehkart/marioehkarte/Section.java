package de.marioehkart.marioehkarte;

public class Section {

    private class _Rail {
        Rail rail;
        double angle;
    }

    private Coordinates startPosition;
    private Coordinates endPosition;
    private _Rail rails[];
    private int railCount;
    private double totalLength;

    private Coordinates railCoordinates(_Rail rail, double distance) {
        Coordinates result = new Coordinates();
        result = Coordinates.transformCoordinates(rail.rail.coordinates(distance), rail.angle);
        return result;
    }

    public Section(Coordinates start, int amount) {
        this.startPosition = start;
        this.endPosition = start;
        this.railCount = 0;
        this.rails = new _Rail[amount];
        this.totalLength = 0;
        for (int i = 0; i < amount; i++) {
            this.rails[i] = new _Rail();
        }
    }

    public void addRail(Rail rail) {
        if (railCount >= rails.length) {
            throw new IllegalArgumentException("Rail array is full");
        } else {
            rails[railCount].rail = rail;
            rails[railCount].angle = this.endPosition.angle_x;
            railCount++;
            totalLength += rail.length();
            endPosition = Coordinates.addCoordinates(endPosition, rail.coordinates(rail.length()));
        }
    }

    public Coordinates position(double distance) {
        if (distance > totalLength) {
            throw new IllegalArgumentException("Distance exceeds section length");
        }
        Coordinates result = new Coordinates();
        // result = Coordinates.addCoordinates(result, startPosition);
        result.x = startPosition.x;
        result.y = startPosition.y;
        result.angle_x = 0;

        for (int i = 0; i < railCount; i++) {
            if (distance > rails[i].rail.length()) {
                distance -= rails[i].rail.length();
                result = Coordinates.addCoordinates(result, railCoordinates(rails[i], rails[i].rail.length()));
            } else {
                result = Coordinates.addCoordinates(result, railCoordinates(rails[i], distance));
                result.angle_x = railCoordinates(rails[i], distance).angle_x;
                if (result.angle_x > Math.PI) {
                    result.angle_x -= 2 * Math.PI;
                } else if (result.angle_x < -Math.PI) {
                    result.angle_x += 2 * Math.PI;
                }
                return result;
            }
        }

        throw new IllegalArgumentException("No rail found for the given distance");
    }

    public double length() {
        return this.totalLength;
    }

}
