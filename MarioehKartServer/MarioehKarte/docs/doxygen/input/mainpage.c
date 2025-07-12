/** @mainpage Overview
 * 
 * 
 * This library enables the creation of a digital map from a Carrera Digital 124/132 racetrack. 
 * It's needed for the Mariööööhh-Kart_v2 project to translate linear position data to cartesian coordinates.
 * 
 * ## Input
 * The whole racetrack must be created by hand with the Carrera-Mapping-Tool.
 * * @htmlonly
 * <a href="../../../Carrera-Mapping-Tool.xlsx" download>Carrera-Mapping-Tool</a>
 * @endhtmlonly
 * 
 * The map is built out from different track sections. Each track section represents one part of the track but only one lane.
 * In the library all standard Carrera 124 rails are available. For each curve there are four parts available. An inner and an outer curve for each right and left direction.
 * Switches are not taken into account. Pit stops and control unit lane must be represented with a standard straight rail, becaus the have the same measuring.
 * 
 * ### Curve syntax
 * e.g. **curve_R1_30_inner_**
 * 
 * | name   | explanation               |
 * |--------|---------------------------|
 * | "curve"| rail type: curve          |
 * | "R1"   | curve radius: R1          |
 * | "30"   | curve angle: 30°          |
 * | "inner"| inner lane of the curve   |
 * | "_"    | right curve               |
 * 
  * e.g. **curve_R4_15_outer**
 * 
 * | name   | explanation               |
 * |--------|---------------------------|
 * | "curve"| rail type: curve          |
 * | "R4"   | curve radius: R4          |
 * | "15"   | curve angle: 15°          |
 * | "outer"| outer lane of the curve   |
 * | " "    | left curve               |
 * 
 * A list with all  available rails can be found @ref rail_definitions "here".
 * 
 * 
 * # Instructions for the use of the Carrera-Mapping-Tool
 * 
 * @warning If you forget the keywords "SECTION_START" and "SECTION_END" or declare a false rail amount, the program won't work!
 * 
 * 
 * 1. Before Listing the Track you have to define a reference coordinate system for the racetrack.
 * 2. Open Carrera-Mapping-Tool.xlsx with Microsoft Excel.
 * 3. Move to page "Vorlage zur Erstellung CSV".
 * 4. List your predefined sections: 
 * Each section has to begin with the keyword "SECTION_START". Then enter the following parameters:
 *  
 *      - barcode: section identifications number
 *      - amount: amount of rails in the section
 *      - pos_x, pos_y: cartestian coordinates in mm from the start point of the section. In relation to the reference coordinate system of the whole track.
 *      - rot_z: rotation in degrees of the start direction in relation to the x-axis
 * 
 *      @image html CMT_empty.png "Carrera-Mapping-Tool"
 *      @image latex CMT_empty.png "Carrera-Mapping-Tool"
 *      @image rtf CMT_empty.png "Carrera-Mapping-Tool"
 * 
 * 5. List all rails of the section in the right order. You can select all possible rails from the dropdown menu.
 * @image html CMT_dropdown.png "You can see all possible rails from the dropdown menu"
 * @image latex CMT_dropdown.png "You can see all possible rails from the dropdown menu"
 * @image rtf CMT_dropdown.png "You can see all possible rails from the dropdown menu"
 * 
 * 6. Each section must be terminated with the keyword "SECTION_END".
 * 7. Repeat steps 4 to 6 until all sections of the track are listed. The order of the sections is not relevant.
 * 
 * 
 * 
 * @image html CMT_example.png "Example of one listed track section."
 * @image latex CMT_example.png "Example of one listed track section."
 * @image rtf CMT_example.png "Example of one listed track section."

 */