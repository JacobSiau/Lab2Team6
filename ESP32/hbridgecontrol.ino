#include <Arduino.h>

const int AIN1 = 25;
const int AIN2 = 26;
const int BIN1 = 27;
const int BIN2 = 28;

void setup() {

    // set up serial comms baud rate, wait for connection to be established
    Serial.begin (115200);
    delay(10);

    // set the pins as outputs:
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);

    // set up PWM pins
    ledcAttachPin(AIN1, 1);
    ledcAttachPin(AIN2, 2);
    ledcAttachPin(BIN1, 3);
    ledcAttachPin(BIN2, 4);

    // set up as (channel, frequency, resolution)
    ledcSetup(1, 12000, 8);
    ledcSetup(2, 12000, 8);
    ledcSetup(3, 12000, 8);
    ledcSetup(4, 12000, 8);

    // set all pins initially off
    ledcWrite(1, 0);
    ledcWrite(2, 0);
    ledcWrite(3, 0);
    ledcWrite(4, 0);

}

void loop() {

    for (int i = 0; i < 5; i++) {
        Hbridge(i);
        Serial.println("sending signal for state %d", i);
        delay(100);
    }

}

void Hbridge (int direction) {
  
    switch(direction) {
        case 0: {
            ledcWrite(1, 0);
            ledcWrite(2, 0);
            ledcWrite(3, 0);
            ledcWrite(4, 0);
            Serial.println("Case: Stop");
        }
        case 1: {
            if (bias<0) {
                ledcWrite(1, 255);
                ledcWrite(2, 0);
                ledcWrite(3, 0);
                ledcWrite(4, 255);
            }
            else {
                ledcWrite(1, 255);
                ledcWrite(2, 0);
                ledcWrite(3, 0);
                ledcWrite(4, 255);
            }
            Serial.println("Case: Forward");
        }
        case 2: {
            ledcWrite(1, 255);
            ledcWrite(2, 0);
            ledcWrite(3, 255);
            ledcWrite(4, 0);
            Serial.println("Case: Left");
        }
        case 3: {
            ledcWrite(1, 0);
            ledcWrite(2, 255);
            ledcWrite(3, 0);
            ledcWrite(4, 255);
            Serial.println("Case: Right");
        }
        case 4: {
            ledcWrite(1, 0);
            ledcWrite(2, 255);
            ledcWrite(3, 255);
            ledcWrite(4, 0);
            Serial.println("Case: Backwards");
        }
    }
} // END Hbridge