#include <Arduino.h>
#include <Wire.h>

#include "StructToReceive.h"

uint32_t rxDataTimer;
const uint16_t rxPeriod = 500; // Receive data from slave every 2 seconds.
const uint16_t slaveAddress = 26; // Addres of the slave device - set in the slave programm - must be different for each slave.
#define TIMEOUT 10

void setup() {

    Wire.begin(21, 22, 400000); // Initializes I2C_0 as a Master. If you want to use I2C_1 -> go with Wire1 instead of Wire.
    Wire.setTimeOut(TIMEOUT); // Default timeout is 1000ms - really really long.

    Serial.begin(115200); // Serial port to send received data to PC and send commands gotten from pc to the slave(s).
    Serial.setTimeout(TIMEOUT); // The same as above.

    Serial.println("\nWaiting for commands:");
    Serial.println(" 1   - Turn on led on slave,");
    Serial.println(" 2   - Turn off led on slave,");
    Serial.println(" 123 - Start led blinking on slave\n");
}

void loop() {

    // Send command from PC to Slave if available:
    if (Serial.available()) {

        uint8_t commandNum = (uint8_t)Serial.readString().toInt();
        Wire.beginTransmission(slaveAddress);
        Wire.write(&commandNum, sizeof(commandNum));
        Wire.endTransmission();
    }

    // Receive data frame from Slave:
    if (millis() - rxDataTimer > rxPeriod) {

        rxDataTimer = millis(); // Reset the timer.

        StructToReceive rxStruct;
        Wire.requestFrom(slaveAddress, sizeof(rxStruct));
        size_t bytesNum = Wire.readBytes((uint8_t*) &rxStruct, sizeof(rxStruct));

        // If received succesfully:
        if (bytesNum) {

            // Print received data:
            Serial.printf("Led state: %d\n",          (int) rxStruct.ledState);
            Serial.printf("Last command: %d\n",       (int) rxStruct.lastDoneCommandNum);
            Serial.printf("Voltage on ADC pin: %f\n", rxStruct.adcVoltage);
            Serial.printf("Value on ADC pin: %d\n\n", (int) rxStruct.adcValue);
        }
        else {

            Serial.println("Slave does not respond");
        }
    }
}