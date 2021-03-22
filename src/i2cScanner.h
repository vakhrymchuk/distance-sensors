#pragma once

#include <Wire.h>

byte checkWire(TwoWire wire) {
    byte working = 0;
    byte count = 0;
    wire.begin();
    for (byte i = 8; i < 120; i++) {
        wire.beginTransmission(i);
        if (wire.endTransmission() == 0) {
            Serial.print("Found address: ");
            Serial.print(i, DEC);
            Serial.print(" (0x");
            Serial.print(i, HEX);
            Serial.println(")");
            count++;
            delay(10);
            working = i;
        }
    }
    Serial.println("Done.");
    Serial.print("Found ");
    Serial.print(count, DEC);
    Serial.println(" device(s).");
//    wire.end();
    return working;
}