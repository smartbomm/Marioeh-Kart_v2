#include "SimpleDEBUG.h"

void SimpleDEBUG::begin(unsigned long baud) {
    Serial.begin(baud);
    while (!Serial) { ; }
}

void SimpleDEBUG::print(const char* msg) {
    Serial.print(msg);
}

void SimpleDEBUG::println(const char* msg) {
    Serial.println(msg);
}

void SimpleDEBUG::print(int value) {
    Serial.print(value);
}

void SimpleDEBUG::println(int value) {
    Serial.println(value);
}
