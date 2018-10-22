// https://forum.arduino.cc/index.php?topic=158385.0

#define enc0A // pick a pin
#define enc0B // pick a pin 

volatile long enc0Pos = 0;
long newpos;
long oldpos = 0;
unsigned long newtime;
unsigned long oldtime = 0;
long vel;

void setup() {
    pinMode(enc0A, INPUT);
    digitalWrite(enc0A, HIGH); // pullup R
    pinMode(enc0B, INPUT);
    digitalWrite(enc0B, HIGH); // pullup R
    attachInterrupt(0, updateEncoder, RISING); // encoder on pin whatever
    Serial.begin(115200); // serial beginning baud
    Serial.println("Begin!");
}

void loop() {
    newpos = enc0Pos;
    newtime = millis();
    vel = (newpos - oldpos) * 1000 / (newtime - oldtime);
    Serial.println("Vel = " + str(vel));
    oldpos = newpos;
    oldtime = newtime;
    delay(250);
}

void updateEncoder() {
    if (digitalRead(enc0A) == digitalRead(enc0B)) {
        enc0Pos++;
    }
    else {
        enc0Pos--;
    }
}