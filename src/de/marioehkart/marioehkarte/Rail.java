package de.marioehkart.marioehkarte;
public class Rail {

    private double length;
    private double radius;
    private double angle;
    
    public Rail(double l, double r) 
    {
        this.length = l;
        this.radius = r;
        angle = r/l;
    }
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
    public double length() {
        return this.length;
    }

    public static Rail straight_standard = new Rail(345, 0);
    public static Rail straight_standard_third = new Rail(115, 0);
    public static Rail straight_standard_fourth = new Rail(86.3, 0);

    public static Rail curve_R1_30_inner = new Rail(129.6, 247.5);
    public static Rail curve_R1_30_inner_ = new Rail(129.6, -247.5);
    public static Rail curve_R1_30_outer = new Rail(181.4, 346.5);
    public static Rail curve_R1_30_outer_ = new Rail(181.4, -346.5);

    public static Rail curve_R1_60_inner = new Rail(259.2, 247.5);
    public static Rail curve_R1_60_inner_ = new Rail(259.2, -247.5);
    public static Rail curve_R1_60_outer = new Rail(362.9, 346.5);
    public static Rail curve_R1_60_outer_ = new Rail(362.9, -346.5);

    public static Rail curve_R2_30_inner = new Rail(233.3, 445.5);
    public static Rail curve_R2_30_inner_ = new Rail(233.3, -445.5);
    public static Rail curve_R2_30_outer = new Rail(285.1, 544.5);
    public static Rail curve_R2_30_outer_ = new Rail(285.1, -544.5);

    public static Rail curve_R3_30_inner = new Rail(336.9, 643.5);
    public static Rail curve_R3_30_inner_ = new Rail(336.9, -643.5);
    public static Rail curve_R3_30_outer = new Rail(388.8, 742.5);
    public static Rail curve_R3_30_outer_ = new Rail(388.8, -742.5);

    public static Rail curve_R4_15_inner = new Rail(220.3, 841.5);
    public static Rail curve_R4_15_inner_ = new Rail(220.3, -841.5);
    public static Rail curve_R4_15_outer = new Rail(246.2, 940.5);
    public static Rail curve_R4_15_outer_ = new Rail(246.2, -940.5);

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

}

