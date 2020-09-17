#include <Arduino.h>
#include <Interval.h>
#include <Wire.h>
#include <TCA9548A.h>
#include "Vl53l0xSensorI2cMux.h"
#include "Vl53l1xSensorI2cMux.h"


TCA9548A i2cMux;

DistanceSensor *sensor1;
DistanceSensor *sensor2;

Interval showInterval(100);
Interval readInterval(100);

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


void setupVl53() {
    checkWire(Wire);

    sensor1 = new Vl53l0xSensorI2cMux(0, &i2cMux);
    sensor2 = new Vl53l1xSensorI2cMux(1, &i2cMux);

    ((Vl53l0xSensor *) sensor1)->initSensor();
    ((Vl53l1xSensor *) sensor2)->initSensor();

}

void setup() {

    Serial.begin(115200);

    Wire.begin();
    Wire.setClock(400000);

    i2cMux.begin(Wire);
    i2cMux.closeAll();
    checkWire(Wire);

    setupVl53();

}

unsigned short l;
unsigned short r;

void loop() {

    if (readInterval.isReady()) {
        l = sensor1->getDistance();
        r = sensor2->getDistance();
    }

    if (showInterval.isReady()) {

        Serial.print("\tleft = ");
        Serial.print(l);
        Serial.print("\tright = ");
        Serial.print(r);

        Serial.println();

    }
}
