package de.marioehkart.marioehkarte;
//! @author JBommeli
//! @version 1.1.5
//! @date 2025-07-13
//! @brief This class represents a section of a map containing rails and their coordinates.
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

    //! @brief Constructor for the Section class.
    //! @param start The starting coordinates of the section.
    //! @param amount The number of rails in the section.
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

    //! @brief Adds a rail to the section.
    //! @param rail The rail to be added.
    //! @throws IllegalArgumentException if more rails than the initialized amount shall be added.
    //! @details This method adds a rail to the section and increases the total length of the section.
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

    //! @ brief Get the coordinate on any position of the section.
    //! @param distance The distance along the section from the start position.
    //! @return The coordinates at the specified distance along the section.
    //! @throws IllegalArgumentException if the parameter distance exceeds the total length of the section.
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

    //! @brief Gets the total length of the section.
    //! @return The total length of the section.
    public double length() {
        return this.totalLength;
    }

}
