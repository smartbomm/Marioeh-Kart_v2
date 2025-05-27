#!/bin/bash
echo "Compiling MarioehKart for Java version $1..."

javac --release "$1" -d bin src/de/marioehkart/marioehkarte/*.java
jar cf MarioehKart_v1_1-jdk"$1".jar -C bin .
cd examples
echo "javac -cp ../MarioehKart_v1_1-jdk$1.jar ./MapToCsv.java"
javac -cp ../MarioehKart_v1_1-jdk"$1".jar ./MapToCsv.java
echo "java -cp ../MarioehKart_v1_1-jdk$1.jar:. MapToCsv ../CSV-Beispiele/Bahn_Abschnitt_1.csv ../test/output 10"
java -cp ../MarioehKart_v1_1.jar-jdk"$1":. MapToCsv ../CSV-Beispiele/Bahn_Abschnitt_1.csv ../test/output 10