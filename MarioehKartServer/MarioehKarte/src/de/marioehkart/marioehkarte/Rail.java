package de.marioehkart.marioehkarte;

/// @brief A Rail object represents one lane from a Carrera rail
/// 
/// All rail types that are actually implemented in the MariööööhhKart_v2 project are defined static.
/// The following rules are decisive for the naming of the rails:
/// 
///  @section straights Straights
/// | main name                 | description   |
/// |---------------------------|---------------|
/// | straight_standard         | 345 mm        |
/// | straight_standard_third   | 115 mm        |
/// | straight_standard_fourth  | 86.3 mm       |
/// 
/// @section curves Curves
/// 
/// 
/// | Carrera radius | radius in mm |
/// |-------|-----|
/// | R1 | 396 mm |
/// | R2 | 594 mm |
/// | R3 | 792 mm |
/// | R4 | 990 mm |
/// 
/// 
/// 
/// | curve_R1_30_          | inner            | R1, 30°, left |
/// |                       | inner_           | R1, 30°, right|
/// 
/// 
/// @author Joel Bommeli
/// @version v1.1.1
/// @date 2025-07-02
/// 
public class Rail {

    private double length; // length in mm of the specific lane
    private double radius; // radius in mm of the specific lane. If it's a straight lane, radius = 0
    private double angle; // angle in rad, automatically calculated. If angle = 0, it's a straight lane
    
    /// @brief constructor of a rail object
    /// @param l length in mm of the lane
    /// @param r radius in mm of the lane. If it's a straight lane, r= 0
    public Rail(double l, double r) 
    {
        this.length = l;
        this.radius = r;
        angle = r/l;
    }

    /// @brief Get the relative position on a rail in relation to the lane start point
    /// @param position Linear position in mm in relation to the start point of the rail.
    /// @return Returns the relative position in Coordinates format in relation to the start point of the rail.
    public Coordinates coordinates(double position)
    {
        Coordinates coordinates = new Coordinates();
        if (position > this.length)
        {
            throw new IllegalArgumentException("Position exceeds rail length");
        }
        double positionAngle = position / this.radius;
        if(this.angle == 0)
        {
            coordinates.x = position;
            coordinates.y = 0;
            coordinates.angle_x = 0;
        }
        else
        {
            coordinates.x = this.radius * Math.sin(positionAngle);
            coordinates.y = this.radius * (1 - Math.cos(positionAngle));
            coordinates.angle_x = positionAngle;  
        }
        return coordinates;
    }

    /// @brief Get the length of the rail
    /// @return length of the rail in mm
    public double length() {
        return this.length;
    }

    /// @brief Parse Rail type from String.
    /// @param rail Name of the specified rail
    public static Rail fromString(String rail) {
        Rail railType;
        if (rail.equals("straight_standard")) {
            railType = Rail.straight_standard;
        } else if (rail.equals("straight_standard_third")) {
            railType = Rail.straight_standard_third;
        } else if (rail.equals("straight_standard_fourth")) {
            railType = Rail.straight_standard_fourth;
        } else if (rail.equals("curve_R1_30_inner")) {
            railType = Rail.curve_R1_30_inner;
        } else if (rail.equals("curve_R1_30_inner_")) {
            railType = Rail.curve_R1_30_inner_;
        } else if (rail.equals("curve_R1_30_outer")) {
            railType = Rail.curve_R1_30_outer;
        } else if (rail.equals("curve_R1_30_outer_")) {
            railType = Rail.curve_R1_30_outer_;
        } else if (rail.equals("curve_R1_60_inner")) {
            railType = Rail.curve_R1_60_inner;
        } else if (rail.equals("curve_R1_60_inner_")) {
            railType = Rail.curve_R1_60_inner_;
        } else if (rail.equals("curve_R1_60_outer")) {
            railType = Rail.curve_R1_60_outer;
        } else if (rail.equals("curve_R1_60_outer_")) {
            railType = Rail.curve_R1_60_outer_;
        } else if (rail.equals("curve_R2_30_inner")) {
            railType = Rail.curve_R2_30_inner;
        } else if (rail.equals("curve_R2_30_inner_")) {
            railType = Rail.curve_R2_30_inner_;
        } else if (rail.equals("curve_R2_30_outer")) {
            railType = Rail.curve_R2_30_outer;
        } else if (rail.equals("curve_R2_30_outer_")) {
            railType = Rail.curve_R2_30_outer_;
        } else if (rail.equals("curve_R3_30_inner")) {
            railType = Rail.curve_R3_30_inner;
        } else if (rail.equals("curve_R3_30_inner_")) {
            railType = Rail.curve_R3_30_inner_;
        } else if (rail.equals("curve_R3_30_outer")) {
            railType = Rail.curve_R3_30_outer;
        } else if (rail.equals("curve_R3_30_outer_")) {
            railType = Rail.curve_R3_30_outer_;
        } else if (rail.equals("curve_R4_15_inner")) {
            railType = Rail.curve_R4_15_inner;
        } else if (rail.equals("curve_R4_15_inner_")) {
            railType = Rail.curve_R4_15_inner_;
        } else if (rail.equals("curve_R4_15_outer")) {
            railType = Rail.curve_R4_15_outer;
        } else if (rail.equals("curve_R4_15_outer_")) {
            railType = Rail.curve_R4_15_outer_;
        } else {
            throw new IllegalArgumentException("Unknown rail type: " + rail);
        }
        return railType;
    }


/** @defgroup rail_definitions Rail Definitions
 * 
 * ## Straights
 * A standard straight has a length of 345 mm. There are also 1/3 and 1/4 standard straights available.
 * 
 * ## Curves
 * Carrera rails can have four different radii:
 * 
 * | name | radius of the outer edge    |
 * |------|-----------------------------|
 * | R1   | 396 mm                      |
 * | R2   | 594 mm                      |
 * | R3   | 792 mm                      |
 * | R4   | 990 mm                      |
 * 
 * More infos at http://www.schlitzflitzer.de/carrera-schienen.          
 * @{
 */

    /// standard straight 345 mm
    public static Rail straight_standard = new Rail(345, 0);

    /// 1/3 standard straight 115 mm
    public static Rail straight_standard_third = new Rail(115, 0);

    /// 1/4 standard straight 86.3 mm
    public static Rail straight_standard_fourth = new Rail(86.3, 0);


    /// left curve R1 30° inner lane 
    public static Rail curve_R1_30_inner = new Rail(129.6, 247.5);

    /// right curve R1 30° inner lane 
    public static Rail curve_R1_30_inner_ = new Rail(129.6, -247.5);

    /// left curve R1 30° outer lane 
    public static Rail curve_R1_30_outer = new Rail(181.4, 346.5);

    /// right curve R1 30° outer lane 
    public static Rail curve_R1_30_outer_ = new Rail(181.4, -346.5);


    /// left curve R1 60° inner lane 
    public static Rail curve_R1_60_inner = new Rail(259.2, 247.5);

    /// right curve R1 60° inner lane 
    public static Rail curve_R1_60_inner_ = new Rail(259.2, -247.5);

    /// left curve R1 60° outer lane 
    public static Rail curve_R1_60_outer = new Rail(362.9, 346.5);

    /// right curve R1 60° outer lane 
    public static Rail curve_R1_60_outer_ = new Rail(362.9, -346.5);


    /// left curve R2 30° inner lane 
    public static Rail curve_R2_30_inner = new Rail(233.3, 445.5);

    /// right curve R2 30° inner lane 
    public static Rail curve_R2_30_inner_ = new Rail(233.3, -445.5);

    /// left curve R2 30° outer lane 
    public static Rail curve_R2_30_outer = new Rail(285.1, 544.5);

    /// right curve R2 30° outer lane 
    public static Rail curve_R2_30_outer_ = new Rail(285.1, -544.5);


    /// left curve R3 30° inner lane 
    public static Rail curve_R3_30_inner = new Rail(336.9, 643.5);

    /// right curve R3 30° inner lane 
    public static Rail curve_R3_30_inner_ = new Rail(336.9, -643.5);

    /// left curve R3 30° outer lane 
    public static Rail curve_R3_30_outer = new Rail(388.8, 742.5);

    /// right curve R3 30° outer lane 
    public static Rail curve_R3_30_outer_ = new Rail(388.8, -742.5);

    
    /// left curve R4 15° inner lane
    public static Rail curve_R4_15_inner = new Rail(220.3, 841.5);

    /// right curve R4 15° inner lane
    public static Rail curve_R4_15_inner_ = new Rail(220.3, -841.5);

    /// left curve R4 15° outer lane
    public static Rail curve_R4_15_outer = new Rail(246.2, 940.5);

    /// right curve R4 15° outer lane
    public static Rail curve_R4_15_outer_ = new Rail(246.2, -940.5);

    

}

/** @} */

