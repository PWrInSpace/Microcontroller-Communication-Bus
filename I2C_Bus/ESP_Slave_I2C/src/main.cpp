#include <Arduino.h>
#include <Wire.h>
#include <WireSlave.h>

#include "StructToTransmit.h"

StructToTransmit txStruct;

uint8_t ledProgramm;
uint32_t ledTimer;

const uint16_t address = 26;
#define TIMEOUT 10
#define LED_Pin 2

void setup() {

    WireSlave.begin(21, 22, address);
    WireSlave.setTimeout(TIMEOUT);

    WireSlave.onRequest(i2cTxInterrupt);
    WireSlave.onReceive(i2cRxInterrupt);
}

void loop() {

    WireSlave.update();
    delay(1);

    switch (ledProgramm) {

    case 1:
        digitalWrite(LED_Pin, 1);
        break;

    case 2:
        digitalWrite(LED_Pin, 0);
        break;

    case 123:

        if (millis() - ledTimer > 1000) {

            ledTimer = millis();
            digitalWrite(LED_Pin, !digitalRead(LED_Pin));
        }
        break;
    }

    txStruct.ledState = digitalRead(LED_Pin);
    txStruct.lastDoneCommandNum = ledProgramm;
    txStruct.adcValue = analogRead(33);
    txStruct.adcVoltage = (txStruct.adcValue * 3.3) / 4095;
}

void i2cTxInterrupt() {

    WireSlave.write((uint8_t*) &txStruct, sizeof(txStruct));
}

void i2cRxInterrupt(int howMany) {

    if (WireSlave.available()) {

        ledProgramm = (uint8_t)WireSlave.readString().toInt();
    }
}