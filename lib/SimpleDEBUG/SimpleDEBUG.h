#ifndef SIMPLEDEBUG_H
#define SIMPLEDEBUG_H

#include <Arduino.h>

class SimpleDEBUG {
public:
    static void begin(unsigned long baud = 115200);
    static void print(const char* msg);
    static void println(const char* msg);
    static void print(int value);
    static void println(int value);
};

#endif // SIMPLEDEBUG_H
