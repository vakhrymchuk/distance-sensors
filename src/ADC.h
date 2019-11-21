#ifndef SENSORS_ADC_H
#define SENSORS_ADC_H

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

const byte pins[] = {A0, A1, A2, A3};
const byte size = sizeof(pins);
volatile int analogValues[8];

int analogReadInterrupt(byte pin) {
    return analogValues[pin - A0];
}

int readCounter = 0;
int pinIndex = 0;

/*** Interrupt routine ADC ready ***/
ISR(ADC_vect) {
    readCounter++;
    if (readCounter == 3) {
        readCounter = 0;
        analogValues[pins[pinIndex] - A0] = ADCW;
        pinIndex = (pinIndex + 1) % size;
        ADMUX = 0x40 | (pins[pinIndex] - A0);
    }
}

#define analogRead(pin) analogReadInterrupt(pin)

#endif


#endif
