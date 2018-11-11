# 1 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino"
# 1 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino"
# 2 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino" 2
# 3 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino" 2
using std::min;
/////////////////////////////////////////////
// HBridge Macros
#define hStop 83 /* S*/
#define hTurn 84 /* T*/
#define hForward 70 /* F*/
#define hBackward 66 /* B*/
#define hLeft 76 /* L*/
#define hRight 82 /* R*/
#define hLeftW 75 /* K*/
#define hRightW 81 /* Q*/
#define hToggle 77 /* M*/
/////////////////////////////////////////////
// Encoder Macros
#define enc0A 34 /* A2*/
#define enc0B 36 /* A4*/
#define cutoffA 1240 /* ~1 V*/
#define cutoffB 1240 /**/
/////////////////////////////////////////////
// MQTT Variables
IPAddress mqttServer(192, 168, 137, 14);
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
const int Forward[4] = {253, 0, 252, 0};
const int Backward[4] = {0, 253, 0, 252};
const int Right[4] = {220, 0, 0, 210};
const int RightWide[4] = {253, 0, 0, 0};
const int Left[4] = {0, 220, 210, 0};
const int LeftWide[4] = {0, 0, 253, 0};
const int freq = 30000; // PWM output frequency [Hz]
const int res = 8; // resolution for PWM channels [b]
const byte mqttLED = 13; // red LED 
/////////////////////////////////////////////
// Timing Variables
volatile unsigned long start_time = 0;
volatile unsigned long cutoff_time = 10000UL;
volatile int time_ms = 0;
/////////////////////////////////////////////
// Encoder Variables
volatile int enc0Atotal = 0; // tracks A each motion command
volatile int enc0Btotal = 0; // tracks B each motion command
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
// FUNCTIONS
/////////////////////////////////////////////////////////////////////////
// callback is called when an MQTT message is recieved by the client 
void callback(char *topic, byte *payload, unsigned int length)
{
  if (String(topic) == "esp32/dir")
  {
    hBridge((int)payload[0], (int)payload[1] - 48, (int)payload[2] - 48,
            (int)payload[3] - 48, (int)payload[4] - 48);
  }
  else if (String(topic) == "esp32/testdir")
  {
    int encount = (int)payload[4] - 48;
    encount += 10 * ((int)payload[3] - 48);
    encount += 100 * ((int)payload[2] - 48);
    encount += 1000 * ((int)payload[1] - 48);
    hBridge2((int)payload[0], encount);
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
// hBridge2 takes in a direction character and a count and:
// -writes out the corresponding motion command
// -stops when it reads that count on the encoders 
void hBridge2(int dir, int encount)
{
  // Serial.println("hB2");
  // Serial.println(encount);
  if (dir == 83 /* S*/)
  {
    writeOut(Stop);
    return;
  }
  //
  switch (dir)
  {
    case 70 /* F*/:
    {
      //Serial.println("F");
      writeOut(Forward);
      // while (std::min(enc0Atotal, enc0Btotal) < encount)
      // {
      //   readEncoders();
      // }
      break;
    }
    case 66 /* B*/:
    {
      //Serial.println("B");
      writeOut(Backward);
      // while (std::min(enc0Atotal, enc0Btotal) < encount) 
      // {
      //   readEncoders();
      // }
      break;
    }
    case 76 /* L*/:
    {
      //Serial.println("L");
      writeOut(Left);
      break;
    }
    case 82 /* R*/:
    {
      //Serial.println("R");
      writeOut(Right);
      break;
    }
    // case hLeftW:
    // {
    //   Serial.println("hLW");
    //   start_time = millis();
    //   writeOut(LeftWide);
    //   while (enc0Btotal < encount) 
    //   {
    //     //readEncoders();
    //     readEncoderB();
    //     if (enc0Btotal < encount && start_time - millis() > cutoff_time) 
    //     {
    //       break;
    //     }
    //   }
    //   writeOut(Stop);
    //   Serial.println("hLWd");
    //   break;
    // }
    // case hRightW:
    // {
    //   Serial.println("hRW");
    //   start_time = millis();
    //   writeOut(RightWide);
    //   Serial.print("ea: ");
    //   Serial.print(enc0Atotal);
    //   while (enc0Atotal < encount) 
    //   {
    //     Serial.println("REA");
    //     //readEncoders();
    //     readEncoderA();
    //     if (enc0Atotal < encount && (millis() - start_time > cutoff_time)) 
    //     {
    //       break;
    //     }
    //   }
    //   writeOut(Stop);
    //   Serial.println("hRWd");
    //   break;
    // }
    default:
    {
      writeOut(Stop);
    }
  } // END switch
  while (std::min(enc0Atotal, enc0Btotal) < encount)
  {
    readEncoders();
  }
  writeOut(Stop);
  publishAndResetTurns();
}
/////////////////////////////////////////////////////////////////////////
// hBridge takes the following pattern <dir><X000ms><X00ms><X0ms><Xms>
// it then calculates the amount of time to run the command for
// it then writes out the pattern found by the char for that amount of time
void hBridge(int dir, int seconds, int hundredms, int tenms, int ms)
{
  if (dir == 83 /* S*/)
  {
    writeOut(Stop);
    return;
  }
  // calculating the time in ms
  time_ms = (seconds >= 0 && seconds <= 9) ? 1000 * seconds : 0;
  time_ms = (hundredms >= 0 && hundredms <= 9) ? time_ms + 100 * hundredms : 0;
  time_ms = (tenms >= 0 && tenms <= 9) ? time_ms + 10 * tenms : 0;
  time_ms = (ms >= 0 && ms <= 9) ? time_ms + ms : 0;
  // what direction did we get told to go?
  switch (dir)
  {
    case 70 /* F*/:
    {
      writeOut(Forward);
      break;
    }
    case 66 /* B*/:
    {
      writeOut(Backward);
      break;
    }
    case 76 /* L*/:
    {
      writeOut(Left);
      break;
    }
    case 82 /* R*/:
    {
      writeOut(Right);
      break;
    }
    case 75 /* K*/:
    {
      writeOut(LeftWide);
      break;
    }
    case 81 /* Q*/:
    {
      writeOut(RightWide);
      break;
    }
    default:
    {
      writeOut(Stop);
    }
  } // END switch
  // read encoders for time_ms ms
  readEncodersForTime(time_ms);
  // stop and read encoders further for 150ms
  // this accounts for the turning of wheels while stopping 
  writeOut(Stop);
  readEncodersForTime(150);
  publishAndResetTurns();
} // END hBridge
/////////////////////////////////////////////////////////////////////////
// reconnect loops until a connection is established with the MQTT broker
void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.println("MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword))
    {
      Serial.println("MQTT!");
      client.subscribe("esp32/#", 1);
      client.publish("esp32/connected", "ESP32");
      digitalWrite(mqttLED, 0x1);
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
// readEncodersForTime does the following for a duration of time_ms [ms]
// -reads state of encoder input pins and determines if it is reading high/low
// -sets to true if high, else false 
// -counts the number of times it flips the state and saves as encoder counts
void readEncodersForTime(int time_ms)
{
  start_time = millis();
  // while time is less than input time, read encoder states and update 
  while (millis() - start_time <= time_ms)
  {
    if (analogRead(34 /* A2*/) > 1240 /* ~1 V*/)
    {
      stateA = true;
    }
    else
    {
      stateA = false;
    }

    if (analogRead(36 /* A4*/) > 1240 /**/)
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
}
/////////////////////////////////////////////////////////////////////////
// readEncoders does the following:
// -reads state of encoder input pins and determines if it is reading high/low
// -flips state of encoder if necessary
// -counts the number of times it flips the state and saves as encoder counts
void readEncoders()
{
  // Serial.println("readEncoders");
  // Serial.println(enc0Atotal);
  // Serial.println(enc0Btotal);
  if (analogRead(34 /* A2*/) > 1240 /* ~1 V*/)
  {
    stateA = true;
  }
  else
  {
    stateA = false;
  }

  if (analogRead(36 /* A4*/) > 1240 /**/)
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
  client.publish("esp32/turns", "X");
  tempmsg = String(enc0Atotal);
  tempmsg.toCharArray(totalAmsg, tempmsg.length() + 1);
  tempmsg = String(enc0Btotal);
  tempmsg.toCharArray(totalBmsg, tempmsg.length() + 1);
  client.publish("esp32/turns", totalAmsg);
  client.publish("esp32/turns", totalBmsg);
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
    pinMode(Motors[i], 0x02);
    ledcSetup(i, freq, res);
    ledcAttachPin(Motors[i], i);
  }
  pinMode(34 /* A2*/, 0x01);
  pinMode(36 /* A4*/, 0x01);
  pinMode(mqttLED, 0x02);
  digitalWrite(mqttLED, 0x0);
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
  if (!client.connected())
  {
    digitalWrite(mqttLED, 0x0);
    reconnect();
  }
  client.loop();
}
/////////////////////////////////////////////////////////////////////////
