# 1 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino"
# 1 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino"
# 2 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino" 2
# 3 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino" 2
/////////////////////////////////////////////
// HBridge Macros
#define hStop 83 /* S*/
#define hForward 70 /* F*/
#define hBackward 66 /* B*/
#define hLeft 76 /* L*/
#define hRight 82 /* R*/
// #define hLeftB 75    // K
// #define hRightB 81   // Q
#define hToggle 77 /* M*/
#define hPWMa 45 /* -*/
#define hPWMb 43 /* +*/
/////////////////////////////////////////////
// Encoder Macros
#define enc0A 34
#define enc0B 36
#define cutoffA 1240 /* ~1 V*/
#define cutoffB 1240 /* */
/////////////////////////////////////////////
// MQTT Variables 
IPAddress mqttServer(192, 168, 137, 1);
const int mqttPort = 1883;
const char *mqttUser = 
# 25 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino" 3 4
                      __null
# 25 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino"
                          ;
const char *mqttPassword = 
# 26 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino" 3 4
                          __null
# 26 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino"
                              ;
/////////////////////////////////////////////
// WiFi Variables
const char *ssid = "DESKTOP-PTFSVRE 2560";
const char *password = "E404h58]";
WiFiClient espClient;
PubSubClient client(espClient);
/////////////////////////////////////////////
// Motor output:    [A1N1,A1N2,B1N2,B1N1]
const int Motors[4] = {12, 32, 27, 33};
/////////////////////////////////////////////
// HBridge Output Patterns/PWM
const int Stop[4] = {0, 0, 0, 0};
const int Forward[4] = {253, 0, 252, 0};
const int Backward[4] = {0, 253, 0, 252};
const int Right[4] = {253, 0, 0, 252};
const int Left[4] = {0, 253, 252, 0};
const int *Temp; // Temp Pattern Holder
const int freq = 30000; // PWM output frequency [Hz]
const int res = 8; // resolution for PWM channels [b]
/////////////////////////////////////////////
// Extra Variables
byte motionLED = 13; // output LED for motion enabled TRUE/FALSE status
volatile bool motionenabled; // global motion enable/disable status
// Timing Variables
volatile unsigned long prev_time = 0;
volatile unsigned long start_time = 0;
volatile unsigned long current_time = 0;
volatile int time_ms = 0;
/////////////////////////////////////////////
// Encoder Variables
//int pulsecountB = 42; // num pulses for 1 rev of wheel B
//int pulsecountA = 42; // num pulses for 1 rev of wheel A
//volatile int enc0Acount = 0; // tracks A each turn
volatile int enc0Atotal = 0; // tracks A each motion command
//volatile int enc0Bcount = 0;
volatile int enc0Btotal = 0;
//volatile int turnsA = 0; // tracks number of turns
//volatile int turnsB = 0;
String tempmsg = ""; // manipulate this to publish
char turnsAmsg[50];
char totalAmsg[50];
char turnsBmsg[50];
char totalBmsg[50];
volatile bool oldstateA = false;
volatile bool oldstateB = false;
volatile bool stateA = false;
volatile bool stateB = false;
/////////////////////////////////////////////
//////////////// FUNCTIONS //////////////////
/////////////////////////////////////////////////////////////////////////
// callback is called when an MQTT message is recieved 
void callback(char *topic, byte *payload, unsigned int length)
{
  if (String(topic) == "esp32/dir")
  {
    // If we don't send enough of a msg for a time/dir command
    if (length < 5) {
      hBridge((int) payload[0], 2, 0, 0, 0);
    }
    // If we sent enough, e.g. <dir><X000ms><X00ms><X0ms><Xms>
    else {
      hBridge((int)payload[0], (int)payload[1] - 48, (int)payload[2] - 48,
      (int)payload[3] - 48, (int)payload[4] - 48);
    }
    return;
  }
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
// toggleMotion does the following:
// -writes out the Stop command to the motors
// -disables all motion commands in loop() by toggling a global boolean
void toggleMotion()
{
  Serial.println("X");
  if (motionenabled)
  {
    motionenabled = false;
    hBridge(83 /* S*/, 0, 0, 0, 0);
    client.publish("esp32/motion", "DISABLED");
  }
  else
  {
    motionenabled = true;
    client.publish("esp32/motion", "ENABLED");
  }
  digitalWrite(motionLED, motionenabled);
}
/////////////////////////////////////////////////////////////////////////
// hBridge takes the following pattern <dir><X000ms><X00ms><X0ms><Xms>
// it then calculates the amount of time to run the command for 
// it then writes out the pattern found by the char for that amount of time
void hBridge(int dir, int seconds, int hundredms, int tenms, int ms)
{
  // calculating the time in ms
  time_ms = (seconds >= 0 && seconds <= 9) ? 1000 * seconds : 0;
  time_ms = (hundredms >= 0 && hundredms <= 9) ? time_ms + 100 * hundredms : 0;
  time_ms = (tenms >= 0 && tenms <= 9) ? time_ms + 10 * tenms : 0;
  time_ms = (ms >= 0 && ms <= 9) ? time_ms + ms : 0;
  // what direction did we get told to go? 
  switch (dir)
  {
  case 77 /* M*/:
  {
    toggleMotion();
    break;
  }
  case 83 /* S*/:
  {
    writeOut(Stop);
    break;
  }
  case 70 /* F*/:
  {
    Temp = Forward;
    break;
  }
  case 66 /* B*/:
  {
    Temp = Backward;
    break;
  }
  case 76 /* L*/:
  {
    Temp = Left;
    break;
  }
  case 82 /* R*/:
  {
    Temp = Right;
    break;
  }
  } // END switch
  // write out the correct pattern for the correct time 
  writeOut(Temp);
  start_time = millis();
  while (millis() - start_time <= time_ms)
  {
    updateTurns();
  }
  // after writing it out, stop, publish info, and reset vars
  writeOut(Stop);
  start_time = millis();
  while (millis() - start_time <= 150)
  {
    updateTurns();
  }
  publishTurns();
  resetTurns();
} // END hBridge
/////////////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////////
// updateTurns does the following:
// -reads state of encoder input pins and determines if it is reading high/low 
// -flips state of encoder if necessary
// -counts the number of times it flips the state and saves as encoder counts 
void updateTurns()
{
  if (analogRead(34) > 1240 /* ~1 V*/)
  {
    stateA = true;
  }
  else
  {
    stateA = false;
  }

  if (analogRead(36) > 1240 /* */)
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
// publishTurns publishes the number of turns read on the encoders
void publishTurns()
{
  client.publish("esp32/turns", "X");
  // tempmsg = String(turnsA);
  // tempmsg.toCharArray(turnsAmsg, tempmsg.length() + 1);
  // tempmsg = String(turnsB);
  // tempmsg.toCharArray(turnsBmsg, tempmsg.length() + 1);
  tempmsg = String(enc0Atotal);
  tempmsg.toCharArray(totalAmsg, tempmsg.length() + 1);
  tempmsg = String(enc0Btotal);
  tempmsg.toCharArray(totalBmsg, tempmsg.length() + 1);
  // client.publish("esp32/turns", turnsAmsg);
  // client.publish("esp32/turns", turnsBmsg);
  client.publish("esp32/turns", totalAmsg);
  client.publish("esp32/turns", totalBmsg);
}
/////////////////////////////////////////////////////////////////////////
// resetTurns resets the global encoder/turn variables
void resetTurns()
{
  // turnsA = 0;
  // turnsB = 0;
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
  // 1. set pinmodes (pin, OUTPUT/INPUT)
  // 2. set up PWM channels (channel, frequency, resolution)
  // 3. set pins to PWM-enabled channels (pin, channel)
  for (int i = 0; i < 4; i++)
  {
    pinMode(Motors[i], 0x02);
    ledcSetup(i, freq, res);
    ledcAttachPin(Motors[i], i);
  }
  // setting up the motion led/variable
  pinMode(motionLED, 0x02);
  pinMode(34, 0x01);
  pinMode(36, 0x01);
  motionenabled = true;
  digitalWrite(motionLED, 0x1);
  // begin connecting to WiFi
  WiFi.begin(ssid, password);
  // loop until connected
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to WiFi");
  // once connected to WiFi, set MQTT for connection
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}
/////////////////////////////////////////////////////////////////////////
// loop is the main loop function of the Arduino program
void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}
/////////////////////////////////////////////////////////////////////////
