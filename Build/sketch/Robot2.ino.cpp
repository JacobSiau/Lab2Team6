#include <Arduino.h>
#line 1 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\Robot2\\Robot2.ino"
#line 1 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\Robot2\\Robot2.ino"
#include <WiFi.h>
#include <PubSubClient.h>
using std::min;
/////////////////////////////////////////////
// HBridge Macros
#define hStop 83     // S
#define hTurn 84     // T
#define hForward 70  // F
#define hForwardSlow 71 // G
#define hBackward 66 // B
#define hLeft 76     // L
#define hRight 82    // R
#define hLeftW 75    // K
#define hRightW 81   // Q
/////////////////////////////////////////////
// PWM Macros
#define SMOTORA 85 // U
#define SMOTORB 86 // V
#define MOTORA 87 // W
#define MOTORAT 88 // X
#define MOTORB 89 // Y
#define MOTORBT 90 // Z
/////////////////////////////////////////////
// Encoder Macros
#define enc0A 14 
#define enc0B 21
#define cutoffA 1240 // ~1 V
#define cutoffB 1240 //
/////////////////////////////////////////////
// IR Pin 
#define IRpin 34
/////////////////////////////////////////////
// MQTT Variables
IPAddress mqttServer(192, 168, 137, 195);
const int mqttPort = 1883;
const char *mqttUser = NULL;
const char *mqttPassword = NULL;
/////////////////////////////////////////////
// WiFi Variables
const char *swarmID = "TTU Swarm Field";
const char *swarmpass = "ttuswarmfield";
WiFiClient espClient;
PubSubClient client(espClient);
/////////////////////////////////////////////
// Motor output:    [A1N1,A1N2,B1N2,B1N1]
const int Motors[4] = {12, 32, 27, 33};
/////////////////////////////////////////////
// HBridge Output Patterns/PWM
const int Stop[4] = {0, 0, 0, 0};
const int Brake[4] = {255, 255, 255, 255};
const int Forward[4] = {250, 0, 253, 0};
const int Backward[4] = {0, 252, 0, 253};
const int Right[4] = {220, 0, 0, 210};
const int Left[4] = {0, 220, 210, 0};
const int RightWide[4] = {250, 0, 0, 0};
const int LeftWide[4] = {0, 0, 252, 0};
/////////////////////////////////////////////
// Dynamic Output Patterns/PWM
volatile int pwmA = 253;
volatile int pwmB = 254;
volatile int slowpwmA = 130;
volatile int slowpwmB = 144;
volatile int pwmAT = 220;
volatile int pwmBT = 222;
const int freq = 1000; // PWM output frequency [Hz]
const int res = 8;      // resolution for PWM channels [b]
const byte mqttLED = 13; // red LED 
/////////////////////////////////////////////
// Timing Variables
volatile unsigned long start_time = 0;
volatile unsigned long cutoff_time = 10000UL;
/////////////////////////////////////////////
// Encoder Variables
volatile int enc0Atotal = 0; // tracks A each motion command
volatile int enc0Btotal = 0; // tracks B each motion command
String tempmsg = ""; // manipulate this to publish
char totalAmsg[50];
char totalBmsg[50];
volatile bool oldstateA = false;
volatile bool oldstateB = false;
volatile bool stateA = false;
volatile bool stateB = false;
/////////////////////////////////////////////
// FUNCTIONS
/////////////////////////////////////////////////////////////////////////
// callback is called when an MQTT message is recieved by the client 
#line 87 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\Robot2\\Robot2.ino"
void callback(char *topic, byte *payload, unsigned int length);
#line 128 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\Robot2\\Robot2.ino"
void goForwardUntilBallDetected();
#line 144 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\Robot2\\Robot2.ino"
void updatePWM(int motor, int newPWM);
#line 196 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\Robot2\\Robot2.ino"
void writeOut(const int *src);
#line 205 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\Robot2\\Robot2.ino"
void hBridgeTimed(int dir, int t_1000ms, int t_100ms, int t_10ms, int t_1ms);
#line 261 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\Robot2\\Robot2.ino"
void hBridge3(int dir);
#line 296 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\Robot2\\Robot2.ino"
void hBridge2(int dir, int encount);
#line 371 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\Robot2\\Robot2.ino"
void reconnect();
#line 399 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\Robot2\\Robot2.ino"
void readEncoders();
#line 432 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\Robot2\\Robot2.ino"
void publishAndResetTurns();
#line 448 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\Robot2\\Robot2.ino"
void setup();
#line 478 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\Robot2\\Robot2.ino"
void loop();
#line 87 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\Robot2\\Robot2.ino"
void callback(char *topic, byte *payload, unsigned int length)
{
  String temp_topic = String(topic);
  if (temp_topic == "esp32/m2")
  {
    int encount = (int)payload[4] - 48;
    encount += 10 * ((int)payload[3] - 48);
    encount += 100 * ((int)payload[2] - 48);
    encount += 1000 * ((int)payload[1] - 48);
    hBridge2((int)payload[0], encount);
  }
  else if (temp_topic == "esp32/m2t")
  {
    hBridgeTimed((int)payload[0], (int)payload[1] - 48,
    (int)payload[2] - 48, (int)payload[3] - 48, (int)payload[4] - 48);
  }
  else if (temp_topic == "esp32/r2") 
  {
    hBridge3((int)payload[0]);
  }
  else if (temp_topic == "esp32/p2") 
  {
    int newPWM = (int)payload[3] - 48;
    newPWM += 10 * ((int)payload[2] - 48);
    newPWM += 100 * ((int)payload[1] - 48);
    updatePWM((int)payload[0], newPWM);
  }
  else if (temp_topic == "esp32/findball") 
  {
    // // int Temp[4] = {pwmA, 0, pwmB, 0};
    // Serial.println("findball");
    // goForwardUntilBallDetected();
    // // start_time = millis();
    // // writeOut(Temp);
    // // while (millis() - start_time < 700);
    // // writeOut(Brake);
    // // writeOut(Stop);
  }
}
/////////////////////////////////////////////////////////////////////////
// goForwardUntilBallDetected writes out Forward until it detects the ball
void goForwardUntilBallDetected() 
{
  int Temp[4] = {pwmA, 0, pwmB, 0};
  while (true) 
  {
  // while(analogRead(IRpin) < 3300)
  // {
    Serial.println(analogRead(IRpin));
    writeOut(Temp);
  }
  // client.publish("esp32/status", "fb", true);
  writeOut(Brake);
  writeOut(Stop);
}
/////////////////////////////////////////////////////////////////////////
// updatewPWM(motor, newpwm) 
void updatePWM(int motor, int newPWM) 
{
  String temp = String(newPWM);
  char tempmsg[50]; 
  switch (motor) 
  {
      case MOTORA:
      {
          pwmA = newPWM;
          temp += "R2A";
          break;
      }
      case MOTORAT:
      {
          pwmAT = newPWM;
          temp += "R2AT";
          break;
      }
      case MOTORB: 
      {
          pwmB = newPWM;
          temp += "R2B";
          break;
      }
      case MOTORBT:
      {
          pwmBT = newPWM;
          temp += "R2BT";
          break;
      }
      case SMOTORA:
      {
          slowpwmA = newPWM;
          temp += "R2AS";
          break;
      }
      case SMOTORB:
      {
          slowpwmB = newPWM;
          temp += "R2BS";
          break;
      }
      default:
      {
          temp += "R2FAIL";
      }
  }
  temp.toCharArray(tempmsg, temp.length() + 1);
  client.publish("esp32/s", tempmsg, true);
}
/////////////////////////////////////////////////////////////////////////
// writeOut uses ledcWrite to write out the pattern found at src
void writeOut(const int *src)
{
  for (int i = 0; i < 4; i++)
  {
    ledcWrite(i, src[i]);
  }
}
/////////////////////////////////////////////////////////////////////////
// hBridgeTimed is hbridge for time 
void hBridgeTimed(int dir, int t_1000ms, int t_100ms, int t_10ms, int t_1ms)
{
  int time_ms = (t_1000ms >= 0 && t_1000ms <= 9) ? 1000 * t_1000ms : 0;
  time_ms = (t_100ms >= 0 && t_100ms <= 9) ? time_ms + 100 * t_100ms : 0;
  time_ms = (t_10ms >= 0 && t_10ms <= 9) ? time_ms + 10 * t_10ms : 0;
  time_ms = (t_1ms >= 0 && t_1ms <= 9) ? time_ms + t_1ms : 0;
  switch (dir) 
  {
    case hForward:
    {
      int Temp[4] = {pwmA, 0, pwmB, 0};
      writeOut(Temp);
      break;
    }
    case hBackward:
    {
      int Temp[4] = {0, pwmA, 0, pwmB};
      writeOut(Temp);
      break;
    }
    case hLeft: 
    {
      int Temp[4] = {0, pwmA, pwmB, 0};
      writeOut(Temp);
      break;
    }
    case hRight: 
    {
      int Temp[4] = {pwmA, 0, 0, pwmB};
      writeOut(Temp);
      break;
    }
    case hLeftW:
    {
      int Temp[4] = {255, 255, pwmBT, 0};
      writeOut(Temp);
      break;
    }
    case hRightW:
    {
      int Temp[4] = {pwmAT, 0, 255, 255};
      writeOut(Temp);
      break;
    }
    default:
    {
      writeOut(Stop);
    }
  }
  start_time = millis();
  while (millis() - start_time <= time_ms);
  writeOut(Brake);
  writeOut(Stop);
}
/////////////////////////////////////////////////////////////////////////
// hBridge3 is simple slow motion 
void hBridge3(int dir) 
{
  switch (dir)
  {
    case hStop: // S
    {
      writeOut(Brake);
      writeOut(Stop);
      break;
    }
    case hForwardSlow:
    {
      int Temp[4] = {slowpwmA, 0, slowpwmB, 0};
      writeOut(Temp);
      break;
    }
    case hForward: // F
    {
      int Temp[4] = {pwmA, 0, pwmB, 0};
      writeOut(Temp);
      break;
    }
    case hBackward: // B
    {
      int Temp[4] = {0, slowpwmA, 0, slowpwmB};
      writeOut(Temp);
      break;
    }
  }
  //publishAndResetTurns();
}
/////////////////////////////////////////////////////////////////////////
// hBridge2 takes in a direction character and a count and:
// -writes out the corresponding motion command
// -stops when it reads that count on the encoders 
void hBridge2(int dir, int encount)
{
  switch (dir)
  {
    case hForward:
    {
      int Temp[4] = {pwmA, 0, pwmB, 0};
      writeOut(Temp);
      while (std::min(enc0Atotal, enc0Btotal) < encount)
      {
        readEncoders();
      }
      break;
    }
    case hBackward:
    {
      int Temp[4] = {0, pwmA, 0, pwmB};
      writeOut(Temp);
      while (std::min(enc0Atotal, enc0Btotal) < encount) 
      {
        readEncoders();
      }
      break;
    }
    case hLeft: 
    {
      int Temp[4] = {0, pwmAT, pwmBT, 0};
      writeOut(Temp);
      while (std::min(enc0Atotal, enc0Btotal) < encount) 
      {
        readEncoders();
      }
      break;
    }
    case hRight: 
    {
      int Temp[4] = {pwmAT, 0, 0, pwmBT};
      writeOut(Temp);
      while (std::min(enc0Atotal, enc0Btotal) < encount) 
      {
        readEncoders();
      }
      break;
    }
    case hLeftW:
    {
      int Temp[4] = {255, 255, pwmBT, 0};
      writeOut(Temp);
      while (enc0Btotal < encount) 
      {
        readEncoders();
      }
      break;
    }
    case hRightW:
    {
      int Temp[4] = {pwmAT, 0, 255, 255};
      writeOut(Temp);
      while (enc0Atotal < encount) 
      {
        readEncoders();
      }
      break;
    }
    default:
    {
      writeOut(Stop);
    }
  } // END switch
  writeOut(Brake);
  writeOut(Stop);
  publishAndResetTurns();
}
/////////////////////////////////////////////////////////////////////////
// reconnect loops until a connection is established with the MQTT broker
void reconnect()
{
  // Loop until we're reconnected
  digitalWrite(mqttLED, LOW);
  while (!client.connected())
  {
    Serial.println("MQTT...");
    if (client.connect("R2", mqttUser, mqttPassword))
    {
      Serial.println("MQTT!");
      client.subscribe("esp32/#", 1);
      client.publish("esp32/c", "R2");
      digitalWrite(mqttLED, HIGH);
    }
    else
    {
      Serial.println("No MQTT. RC:" + String(client.state()));
      Serial.print(client.state());
      // Retry after 5 seconds
      delay(5000);
    }
  }
}
/////////////////////////////////////////////////////////////////////////
// readEncoders does the following:
// -reads state of encoder input pins and determines if it is reading high/low
// -flips state of encoder if necessary
// -counts the number of times it flips the state and saves as encoder counts
void readEncoders()
{
  if (digitalRead(enc0A)) 
  {
      stateA = true;
  }
  else 
  {
      stateA = false;
  }
  if (digitalRead(enc0B)) 
  {
      stateB = true;
  }
  else 
  {
      stateB = false;
  }
  if (oldstateA != stateA)
  {
    enc0Atotal++;
    oldstateA = stateA;
  }

  if (oldstateB != stateB)
  {
    enc0Btotal++;
    oldstateB = stateB;
  }
}
/////////////////////////////////////////////////////////////////////////
// publishAndResetTurns publishes the number of turns read on the encoders
// it then resets the turn variables 
void publishAndResetTurns()
{
  client.publish("esp32/t", "T2", true);
  tempmsg = String(enc0Atotal);
  tempmsg.toCharArray(totalAmsg, tempmsg.length() + 1);
  tempmsg = String(enc0Btotal);
  tempmsg.toCharArray(totalBmsg, tempmsg.length() + 1);
  client.publish("esp32/t", totalAmsg, true);
  client.publish("esp32/t", totalBmsg, true);
  enc0Atotal = 0;
  enc0Btotal = 0;
  stateA = false;
  stateB = false;
}
/////////////////////////////////////////////////////////////////////////
// setup is the main setup function of this Arduino program
void setup()
{
  Serial.begin(115200);
  // ESP32 pins/PWM setup 
  for (int i = 0; i < 4; i++)
  {
    pinMode(Motors[i], OUTPUT);
    ledcSetup(i, freq, res);
    ledcAttachPin(Motors[i], i);
  }
  pinMode(enc0A, INPUT);
  pinMode(enc0B, INPUT);
  pinMode(IRpin, INPUT);
  pinMode(mqttLED, OUTPUT);
  digitalWrite(mqttLED, LOW);
  // begin connecting to WiFi
  WiFi.begin(swarmID, swarmpass);
  // loop until connected
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("WiFi...");
  }
  Serial.println("WiFi!");
  // once connected to WiFi, set up MQTT for connection
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}
/////////////////////////////////////////////////////////////////////////
// loop is the main loop function of this Arduino program
void loop()
{
  if (!client.connected()) reconnect();
  client.loop();
}
/////////////////////////////////////////////////////////////////////////
