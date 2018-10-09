/*
Contributors:
Ian Kriner, Jacob Siau
*/
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

class RClient
{

  private:
    const char *ssid = "DESKTOP-PTFSVRE 2560";
    const char *password = "E404h58]";

  public:
    RClient(){

    };
    void setupClient()
    {
        Serial.println();
        Serial.println();
        Serial.print("Connecting to: ");
        Serial.println(this->ssid);

        WiFi.begin(this->ssid, this->password);

        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
        }

        Serial.println("");
        Serial.println("WiFi connected.");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
    };

    // getRequest performs an HTTP GET request at an input url (command center)
    // getRequest returns a state (-1, 0-6) to be input to hBridge for motor control 
    int getRequest(String url)
    {
        if ((WiFi.status() == WL_CONNECTED))
        {
            HTTPClient http;
            http.begin(url);
            int httpCode = http.GET();
            Serial.println(httpCode);

            if (httpCode == 200)
            {
                char response = http.getString().charAt(0);
                Serial.println("HTTP Successful. Response text: ");
                Serial.println(response);
                if ('s' == response) {
                    return -1;
                }
                return ((int) response - 48);
            }
            else
            {
                Serial.println("Error with HTTP!");
                return -1;
            }
        }
    };
};

const int Motors[4] = {12, 15, 27, 33}; // Motor output pins [A1N1, A1N2, B1N1, B1N2]

const int Stop[4] = {0, 0, 0, 0};         // Stop pattern
const int Forward[4] = {255, 0, 255, 0};  // Forward pattern
const int Backward[4] = {0, 255, 0, 255}; // Backward pattern
const int Left[4] = {255, 0, 0, 255};     // Left pattern
const int LeftBias[4] = {255, 125, 125, 255}; // Left Bias pattern
const int Right[4] = {0, 255, 255, 0};    // Right pattern
const int RightBias[4] = {125, 255, 255, 125}; // Right Bias pattern

const int freq = 30000; // PWM output frequency [Hz]
const int res = 8;      // resolution for PWM channels [b]

byte interruptPin = 14;          // pushbutton interrupt input (also tied to LED1)
byte motionLED = 32;
int currentstate = 0;            // current state for hBridge
volatile bool motionenabled;     // global motion enabling/disabling variable
volatile long debounce_time = 0; // for debouncing
volatile long current_time = 0;

RClient c1 = RClient();

// writeOut does the following:
// 1. uses ledcWrite to write out the pattern in src to the PWM channels
void writeOut(const int *src)
{
    for (int i = 0; i < 4; i++)
    {
        ledcWrite(i, src[i]);
    }
}

// Hbridge takes a integer representing a direction case and writes a pattern based on it
// Cases: -1=Stop, 0=Forward, 1=Backward, 2=Left, 3=Right
// Cases: 4=Left Bias, 5=Right Bias
// void hBridge(int dir, int lbias, int rbias)
void hBridge(int dir)
{
    switch (dir)
    {
    case -1: // -1: Stop
    {
        writeOut(Stop);
        Serial.println("\t[S]");
        break;
    }
    case 0: // 0: Forward
    {
        writeOut(Forward);
        Serial.println("\t[F]");
        break;
    }
    case 1: // 1: Backward
    {
        writeOut(Backward);
        Serial.println("\t[B]");
        break;
    }
    case 2: // 2: Left
    {
        writeOut(Left);
        Serial.println("\t[L]");
        break;
    }
    case 3: // 3: Right
    {
        writeOut(Right);
        Serial.println("\t[R]");
        break;
    }
    case 4: // 4: Left Biased
    {
        writeOut(LeftBias);
        Serial.println("\t[L-B]");
        break;
    }
    case 5: // 5: Right Biased
    {
        writeOut(RightBias);
        Serial.println("\t[R-B]");
        break;
    }
    } // END switch
} // END hBridge

// toggleMotion does the following:
// 1. writes out the Stop command to the motors
// 2. disables all motion commands in loop() by toggling a global boolean
void toggleMotion()
{
    Serial.println("X");
    current_time = millis();
    if ((current_time - debounce_time) > 200)
    {
        if (motionenabled)
        {
            motionenabled = false;
            hBridge(-1);
        }
        else
        {
            motionenabled = true;
        }
    }
    debounce_time = current_time;
    digitalWrite(motionLED, motionenabled);
}

// setup is the setup function that runs once at the start of the Arduino-C program
void setup()
{
    Serial.begin(115200);
    delay(10);

    // 1. setting pinmodes (pin, OUTPUT/INPUT)
    // 2. setting up PWM channels (channel, frequency, resolution)
    // 3. setting pins to PWM-enabled channels (pin, channel)
    for (int i = 0; i < 4; i++)
    {
        pinMode(Motors[i], OUTPUT);
        ledcSetup(i, freq, res);
        ledcAttachPin(Motors[i], i);
    }

    // 1. declaring the interrupt pin location
    // 2. pointing to the ISR to be used upon interrupt on a digital I/O pin
    pinMode(interruptPin, INPUT_PULLDOWN);
    pinMode(motionLED, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(interruptPin), toggleMotion, RISING);
    motionenabled = true;

    c1.setupClient();
}

// loop is the main loop of the Arduino-C program
void loop()
{
    delay(5000);

    if (!motionenabled)
    {
        Serial.println("[~M]");
        hBridge(-1);
    }
    else
    {
        int temp = c1.getRequest("http://192.168.137.1:8000/");
        currentstate = temp;
        hBridge(currentstate % 6);
        // currentstate++;
    }
}