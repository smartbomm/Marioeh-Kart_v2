package de.marioehkart.marioehkarte;
///
/// @brief This class represents a set of coordinates in a 2D space.
/// It includes methods for adding coordinates and transforming them based on an angle.
/// The coordinates are represented by x, y, and an angle in radians.
/// The class provides a constructor for initializing coordinates and methods for adding and transforming them.
///
/// The class is used in the context of a railway system, where coordinates represent positions on the tracks.
/// The coordinates can be used to calculate positions of trains or other objects on the railway.
/// The transformation method allows for rotating the coordinates based on a given angle.
/// The class is part of a larger system that includes sections and rails, which are used to represent the railway tracks.
//! @author Joel Bommeli
//! @date 2025-07-02
//! @version v1.1.1

public class Coordinates {
    /// @brief Default constructor for Coordinates class.
    /// Initializes the coordinates to (0, 0) with an angle of 0 radians.
    /// @param x The x-coordinate.
    /// @param y The y-coordinate.
    /// @param angle_x The angle in radians.
    public Coordinates() {
        this.x = 0;
        this.y = 0;
        this.angle_x = 0;
    }
    /// @brief Constructor for Coordinates class.
    /// Initializes the coordinates with the given x, y, and angle values.
    /// @param x The x-coordinate.
    /// @param y The y-coordinate.
    /// @param angle_x The angle in radians.
    public Coordinates(double x, double y, double angle_x) {
        this.x = x;
        this.y = y;
        this.angle_x = angle_x;
    }
    /// @brief The x-coordinate.
    public double x;
    /// @brief The y-coordinate.
    public double y;
    /// @brief The angle in radians.
    public double angle_x;
    /// @brief Addition of two Coordinates objects.
    /// Adds the x and y coordinates and angles of two Coordinates objects.
    /// @param a The first Coordinates object.
    /// @param b The second Coordinates object.
    /// @return A new Coordinates object with the summed coordinates and angles.
    public static Coordinates addCoordinates(Coordinates a, Coordinates b) {
        Coordinates result = new Coordinates();
        result.x = a.x + b.x;
        result.y = a.y + b.y;
        result.angle_x = a.angle_x + b.angle_x;
        return result;
    }
    /// @brief Transformation of coordinates based on an angle.
    /// Transforms the coordinates of a Coordinates object by rotating them around the origin.
    /// @param a The Coordinates object to be transformed.
    /// @param angle The angle in radians by which to rotate the coordinates.
    /// @return A new Coordinates object with the transformed coordinates.
    public static Coordinates transformCoordinates(Coordinates a, double angle) {
        Coordinates result = new Coordinates();
        result.x = a.x * Math.cos(angle) - a.y * Math.sin(angle);
        result.y = a.x * Math.sin(angle) + a.y * Math.cos(angle);
        result.angle_x = a.angle_x + angle;
        return result;
    }
}
