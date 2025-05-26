#!/bin/bash
javac -d bin src/de/marioehkart/marioehkarte/Coordinates.java src/de/marioehkart/marioehkarte/Map.java src/de/marioehkart/marioehkarte/Rail.java src/de/marioehkart/marioehkarte/Section.java
jar cf MarioehKart_v1_1.jar -C bin .
cd examples
javac -cp ../MarioehKart_v1_1.jar ./MapToCsv.java
java -cp ../MarioehKart_v1_1.jar:. MapToCsv ../CSV-Beispiele/Bahn_Abschnitt_1.csv ../test/output 10