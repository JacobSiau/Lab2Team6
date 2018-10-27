#include <Arduino.h>
#line 1 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino"
#line 1 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino"
#include <WiFi.h>
#include <PubSubClient.h>

#define hStop 83     // S
#define hForward 70  // F
#define hBackward 66 // B
#define hLeft 76     // L
#define hRight 82    // R
#define hLeftB 75    // K
#define hRightB 81   // Q
#define hToggle 77   // M

#define enc0A 34
#define enc0B 39
#define cutoffA 2730
#define cutoffB 2730

const char *ssid = "DESKTOP-PTFSVRE 2560";
const char *password = "E404h58]";
//IPAddress mqttServer(192, 168, 137, 194);
IPAddress mqttServer(192, 168, 137, 219);
// const char* mqttServer = "mqtt://iot.eclipse.org";
// const char* mqttServer = "127.0.0.1";

const int mqttPort = 1883;
const char *mqttUser = NULL;
const char *mqttPassword = NULL;

const int Motors[4] = {12, 15, 27, 33}; // Motor output pins [A1N1, A1N2, B1N1, B1N2]

const int Stop[4] = {0, 0, 0, 0};              // Stop pattern
const int Forward[4] = {255, 0, 255, 0};       // Forward pattern
const int Backward[4] = {0, 255, 0, 255};      // Backward pattern
const int Right[4] = {255, 0, 0, 255};         // Left pattern
const int RightBias[4] = {255, 125, 125, 255}; // Left Bias pattern
const int Left[4] = {0, 255, 255, 0};          // Right pattern
const int LeftBias[4] = {125, 255, 255, 125};  // Right Bias pattern

const int freq = 30000; // PWM output frequency [Hz]
const int res = 8;      // resolution for PWM channels [b]

byte motionLED = 13;             // output LED for motion enabled TRUE/FALSE status
int currentstate = 0;            // current state for hBridge
volatile bool motionenabled;     // global motion enabling/disabling variable
volatile long debounce_time = 0; // for debouncing
volatile long current_time = 0;  // for debouncing

int pulsecountB = 16;
int pulsecountA = 16;
volatile int enc0Acount = 0;
volatile int turnsA = 0;
String tempmsg = "";
char turnsAmsg[50];
char turnsBmsg[50];
volatile int turnsB = 0;
volatile int enc0Bcount = 0;
volatile bool oldstateA = false;
volatile bool oldstateB = false;
volatile bool stateA = false;
volatile bool stateB = false;

WiFiClient espClient;
PubSubClient client(espClient);

#line 65 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino"
void callback(char *topic, byte *payload, unsigned int length);
#line 89 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino"
void writeOut(const int *src);
#line 100 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino"
void toggleMotion();
#line 121 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino"
void hBridge(int dir);
#line 189 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino"
void reconnect();
#line 212 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino"
void setup();
#line 248 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino"
void loop();
#line 65 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino"
void callback(char *topic, byte *payload, unsigned int length)
{

  //Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  if (String(topic) == "esp32/dir")
  {
    //Serial.println("Direction control char: " + (int)payload[0]);
    hBridge((int)payload[0]);
  }
  else
  {
    Serial.println("Other topic!");
  }

  //Serial.print("Message: ");
  for (int i = 0; i < length; i++)
  {
    Serial.println((char)payload[i]);
  }
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
    hBridge(hStop);
    client.publish("esp32/motion", "DISABLED");
  }
  else
  {
    motionenabled = true;
    client.publish("esp32/motion", "ENABLED");
  }
  //}
  //debounce_time = current_time;
  digitalWrite(motionLED, motionenabled);
}

// Hbridge takes a integer representing a direction case and writes a pattern based on it
// void hBridge(int dir, int lbias, int rbias)
void hBridge(int dir)
{
  writeOut(Stop);
  delay(10);
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
    Serial.println("[S]");
    break;
  }
  case hForward:
  {
    writeOut(Forward);
    Serial.println("[F]");
    break;
  }
  case hBackward:
  {
    writeOut(Backward);
    Serial.println("[B]");
    break;
  }
  case hLeft:
  {
    writeOut(Left);
    Serial.println("[L]");
    break;
  }
  case hRight:
  {
    writeOut(Right);
    Serial.println("[R]");
    break;
  }
  case hLeftB:
  {
    writeOut(LeftBias);
    Serial.println("[LB]");
    break;
  }
  case hRightB:
  {
    writeOut(RightBias);
    Serial.println("[RB]");
    break;
  }
  } // END switch
  // Serial.println(turnsA);
  // Serial.println(turnsB);
  client.publish("esp32/turns", "X");
  tempmsg = String(turnsA);
  tempmsg.toCharArray(turnsAmsg, tempmsg.length() + 1);
  tempmsg = String(turnsB);
  tempmsg.toCharArray(turnsBmsg, tempmsg.length() + 1);
  client.publish("esp32/turns", turnsAmsg);
  client.publish("esp32/turns", turnsBmsg);
  turnsA = 0;
  turnsB = 0;
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
      //Serial.println("A");
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
      //Serial.println("B");
    }
    oldstateB = stateB;
  }

} // END main loop
