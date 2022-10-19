#pragma once

//#define FREE_RUN_MODE true

#ifdef FREE_RUN_MODE

void ADC_setup() {
    cli();
    DIDR0 = 0x3F;       // digital inputs disabled
    ADMUX = 0x43;       // measuring on ADC3, use the internal 1.1 reference
    ADCSRA = 0xAC;      // AD-converter on, interrupt enabled, prescaler = 16
    ADCSRB = 0x40;      // AD channels MUX on, free running mode
    bitSet(ADCSRA, 6);  // Start the conversion by setting bit 6 (=ADSC) in ADCSRA
    sei();              // set interrupt flag
}

const byte pins[] = {A0, A1, A2, A6, A7};
const byte size = sizeof(pins);
volatile unsigned short analogValues[size];

int analogReadInterrupt(byte pin) {
    for (int i = 0; i < size; ++i) {
        if (pins[i] == pin) {
            return analogValues[i];
        }
    }
    return 0;
}

byte readCounter = 0;
byte pinIndex = 0;

/*** Interrupt routine ADC ready ***/
ISR(ADC_vect) {
    readCounter++;
    if (readCounter == 3) {
        readCounter = 0;
        analogValues[pinIndex] = ADCW;
        pinIndex = (pinIndex + 1) % size;
        ADMUX = 0x40 | (pins[pinIndex] - A0);
    }
}

#define analogRead(pin) analogReadInterrupt(pin)

#endif

