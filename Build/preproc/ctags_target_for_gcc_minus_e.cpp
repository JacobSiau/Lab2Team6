# 1 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino"
# 1 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino"
# 2 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino" 2
# 3 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino" 2

#define hStop 83
#define hForward 70
#define hBackward 66
#define hLeft 76
#define hRight 82
#define hLeftB 75
#define hRightB 81
#define hToggle 77

const char *ssid = "DESKTOP-PTFSVRE 2560";
const char *password = "E404h58]";
IPAddress mqttServer(192, 168, 137, 194);
// const char* mqttServer = "mqtt://iot.eclipse.org";
// const char* mqttServer = "127.0.0.1";

const int mqttPort = 1883;
const char *mqttUser = 
# 20 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino" 3 4
                      __null
# 20 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino"
                          ;
const char *mqttPassword = 
# 21 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino" 3 4
                          __null
# 21 "c:\\Users\\jacob\\PycharmProjects\\Team6Lab2\\WeMos\\Test1.ino"
                              ;

const int Motors[4] = {12, 15, 27, 33}; // Motor output pins [A1N1, A1N2, B1N1, B1N2]

const int Stop[4] = {0, 0, 0, 0}; // Stop pattern
const int Forward[4] = {255, 0, 255, 0}; // Forward pattern
const int Backward[4] = {0, 255, 0, 255}; // Backward pattern
const int Right[4] = {255, 0, 0, 255}; // Left pattern
const int RightBias[4] = {255, 125, 125, 255}; // Left Bias pattern
const int Left[4] = {0, 255, 255, 0}; // Right pattern
const int LeftBias[4] = {125, 255, 255, 125}; // Right Bias pattern

const int freq = 30000; // PWM output frequency [Hz]
const int res = 8; // resolution for PWM channels [b]

byte interruptPin = 14; // pushbutton interrupt input
byte motionLED = 13; // output LED for motion enabled TRUE/FALSE status
int currentstate = 0; // current state for hBridge
volatile bool motionenabled; // global motion enabling/disabling variable
volatile long debounce_time = 0; // for debouncing
volatile long current_time = 0; // for debouncing

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char *topic, byte *payload, unsigned int length)
{

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  if (String(topic) == "esp32/dir")
  {
    Serial.println("Direction control char: " + (int)payload[0]);
    hBridge((int)payload[0]);
  }
  else
  {
    Serial.println("Other topic!");
  }

  Serial.print("Message: ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }

  Serial.println();
  Serial.println("-----------------------");
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
  current_time = millis();
  if ((current_time - debounce_time) > 800)
  {
    Serial.println("X");
    if (motionenabled)
    {
      motionenabled = false;
      hBridge(83);
      client.publish("esp32/motion", "DISABLED");
    }
    else
    {
      motionenabled = true;
      client.publish("esp32/motion", "ENABLED");
    }
  }
  debounce_time = current_time;
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
  case 77:
  {
    toggleMotion();
    break;
  }
  case 83:
  {
    writeOut(Stop);
    Serial.println("\t[S]");
    break;
  }
  case 70:
  {
    writeOut(Forward);
    Serial.println("\t[F]");
    break;
  }
  case 66:
  {
    writeOut(Backward);
    Serial.println("\t[B]");
    break;
  }
  case 76:
  {
    writeOut(Left);
    Serial.println("\t[L]");
    break;
  }
  case 82:
  {
    writeOut(Right);
    Serial.println("\t[R]");
    break;
  }
  case 75:
  {
    writeOut(LeftBias);
    Serial.println("\t[L-B]");
    break;
  }
  case 81:
  {
    writeOut(RightBias);
    Serial.println("\t[R-B]");
    break;
  }
  } // END switch
} // END hBridge

// reconnect loops until a connection is established with the MQTT broker
void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client"))
    {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/#");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
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
    pinMode(Motors[i], 0x02);
    ledcSetup(i, freq, res);
    ledcAttachPin(Motors[i], i);
  }

  // 1. declaring the interrupt pin location
  // 2. pointing to the ISR to be used upon interrupt on a digital I/O pin
  pinMode(interruptPin, 0x09);
  pinMode(motionLED, 0x02);
  attachInterrupt((((interruptPin)<40)?(interruptPin):-1), toggleMotion, 0x01);
  motionenabled = true;

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

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
}
