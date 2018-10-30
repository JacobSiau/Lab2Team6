#include <WiFi.h>
#include <PubSubClient.h>

#define hStop 83     // S
#define hForward 70  // F
#define hBackward 66 // B
#define hLeft 76     // L
#define hRight 82    // R
// #define hLeftB 75    // K
// #define hRightB 81   // Q
#define hToggle 77 // M
#define hPWMa 45   // -
#define hPWMb 43   // +
//
#define enc0A 34
#define enc0B 39
#define cutoffA 2730
#define cutoffB 2730
//
const char *ssid = "DESKTOP-PTFSVRE 2560";
const char *password = "E404h58]";
IPAddress mqttServer(192, 168, 137, 219);
//
const int mqttPort = 1883;
const char *mqttUser = NULL;
const char *mqttPassword = NULL;
//
const int Motors[4] = {12, 32, 27, 33}; // Motor output pins [A1N1, A1N2, B1N1, B1N2]
//
const int *Temp;
const int Stop[4] = {0, 0, 0, 0};         // Stop pattern
const int Forward[4] = {252, 0, 254, 0};  // Forward pattern
const int Backward[4] = {0, 252, 0, 254}; // Backward pattern
const int Right[4] = {250, 0, 0, 255};    // Left pattern
const int Left[4] = {0, 250, 255, 0};     // Right pattern
//const int RightBias[4] = {255, 125, 125, 255}; // Left Bias pattern
//const int LeftBias[4] = {125, 255, 255, 125};  // Right Bias pattern
//
const int freq = 30000; // PWM output frequency [Hz]
const int res = 8;      // resolution for PWM channels [b]
//
byte motionLED = 13;         // output LED for motion enabled TRUE/FALSE status
volatile bool motionenabled; // global motion enabling/disabling variable
//
volatile unsigned long prev_time = 0;
volatile unsigned long start_time = 0;
volatile unsigned long current_time = 0;
//const unsigned long period = 2000;
volatile int time_ms = 0;
//
int pulsecountB = 16;
int pulsecountA = 16;
volatile int enc0Acount = 0;
volatile int enc0Bcount = 0;
volatile int turnsA = 0;
volatile int turnsB = 0;
String tempmsg = "";
char turnsAmsg[50];
char turnsBmsg[50];
volatile bool oldstateA = false;
volatile bool oldstateB = false;
volatile bool stateA = false;
volatile bool stateB = false;
//
WiFiClient espClient;
PubSubClient client(espClient);
//
void callback(char *topic, byte *payload, unsigned int length)
{
  //Serial.print("Message arrived in topic: ");

  if (String(topic) == "esp32/dir")
  {
    //Serial.println("Direction control char: " + (int)payload[0]);
    if (length < 5) {
      //Serial.println("Payload too small");
      hBridge((int) payload[0], 2, 0, 0, 0);
    }
    else {
      //Serial.println("Payload just right");
      hBridge((int)payload[0], (int)payload[1] - 48, (int)payload[2] - 48,
      (int)payload[3] - 48, (int)payload[4] - 48);
    }
    return;
  }
  // else
  // {
  //   Serial.println("Other topic!");
  // }

  // //Serial.print("Message: ");
  // for (int i = 0; i < length; i++)
  // {
  //   Serial.println((char)payload[i]);
  // }
}

// writeOut uses ledcWrite to write out the pattern at &src to the PWM channels
void writeOut(const int *src)
{
  for (int i = 0; i < 4; i++)
  {
    ledcWrite(i, src[i]);
  }
}

// toggleMotion does the following:
// 1. writes out the Stop command to the motors
// 2. disables all motion commands in loop() by toggling a global boolean
void toggleMotion()
{
  Serial.println("X");
  if (motionenabled)
  {
    motionenabled = false;
    hBridge(hStop, 0, 0, 0, 0);
    client.publish("esp32/motion", "DISABLED");
  }
  else
  {
    motionenabled = true;
    client.publish("esp32/motion", "ENABLED");
  }
  digitalWrite(motionLED, motionenabled);
}

// Hbridge takes a integer representing a direction case and writes a pattern based on it
void hBridge(int dir, int seconds, int hundredms, int tenms, int ms)
{
  time_ms = (seconds >= 0 && seconds <= 9) ? 1000 * seconds : 0;
  time_ms = (hundredms >= 0 && hundredms <= 9) ? time_ms + 100 * hundredms : 0;
  time_ms = (tenms >= 0 && tenms <= 9) ? time_ms + 10 * tenms : 0;
  time_ms = (ms >= 0 && ms <= 9) ? time_ms + ms : 0;
  //writeOut(Stop);
  //delay(10);
  switch (dir)
  {
  case hToggle:
  {
    toggleMotion();
    break;
  }
  case hStop:
  {
    writeOut(Stop);
    //writeOut(Stop);
    //Serial.println("[S]");
    break;
  }
  case hForward:
  {
    Temp = Forward;
    //writeOut(Forward);
    //Serial.println("[F]");
    break;
  }
  case hBackward:
  {
    Temp = Backward;
    //writeOut(Backward);
    // Serial.println("[B]");
    break;
  }
  case hLeft:
  {
    Temp = Left;
    //writeOut(Left);
    // Serial.println("[L]");
    break;
  }
  case hRight:
  {
    Temp = Right;
    //writeOut(Right);
    // Serial.println("[R]");
    break;
  }
  } // END switch
  Serial.println(time_ms);
  start_time = millis();
  while (millis() - start_time <= time_ms)
  {
    writeOut(Temp);
    updateTurns();
  } 
  publishTurns();
  writeOut(Stop);

} // END hBridge

// reconnect loops until a connection is established with the MQTT broker
void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Establishing MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword))
    {
      Serial.println("Success!");
      client.subscribe("esp32/#");
      client.publish("esp32/connected", "connected");
    }
    else
    {
      Serial.print("MQTT failed. State: ");
      Serial.print(client.state());
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void updateTurns()
{
  if (analogRead(enc0A) > cutoffA)
  {
    stateA = true;
  }
  else
  {
    stateA = false;
  }

  if (analogRead(enc0B) > cutoffB)
  {
    stateB = true;
  }
  else
  {
    stateB = false;
  }

  if (oldstateA != stateA)
  {
    enc0Acount++;
    if (enc0Acount > pulsecountA)
    {
      enc0Acount = 0;
      turnsA++;
    }
    oldstateA = stateA;
  }

  if (oldstateB != stateB)
  {
    enc0Bcount++;
    if (enc0Bcount > pulsecountB)
    {
      enc0Bcount = 0;
      turnsB++;
    }
    oldstateB = stateB;
  }
}

// publishturns published the number of turns read on the encoders
void publishTurns()
{
  client.publish("esp32/turns", "X");
  tempmsg = String(turnsA);
  tempmsg.toCharArray(turnsAmsg, tempmsg.length() + 1);
  tempmsg = String(turnsB);
  tempmsg.toCharArray(turnsBmsg, tempmsg.length() + 1);
  client.publish("esp32/turns", turnsAmsg);
  client.publish("esp32/turns", turnsBmsg);
  turnsA = 0;
  turnsB = 0;
}

// the main setup function of the Arduino program
void setup()
{
  Serial.begin(115200);

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
  pinMode(motionLED, OUTPUT);
  // pinMode(enc0A, INPUT);
  // pinMode(enc0B, INPUT);
  motionenabled = true;
  digitalWrite(motionLED, HIGH);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to WiFi");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

// the main loop function of the Arduino program
void loop()
{
  if (!client.connected())
  {
    reconnect();
  }

  client.loop();

} // END main loop