#include <WiFi.h>
#include <HTTPClient.h>

// const char *ssid = "DESKTOP-PTFSVRE 2560";
// const char *password = "E404h58]";

class RClient
{

private:
  const char *ssid = "DESKTOP-PTFSVRE 2560";
  const char *password = "E404h58]";

public:
  void setupClient(void);
  void getRequest(String);

};

void RClient::setupClient()
{
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(this.ssid);

  WiFi.begin(this.ssid, this.password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void RClient::getRequest(String url)
{
  delay(2000);

  if ((WiFi.status() == WL_CONNECTED))
  {
    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0)
    {
      String payload = http.getString();
      Serial.println("HTTP > 0");
      Serial.println(payload);
    }
    else
    {
      Serial.println("Error with HTTP!");
    }
    Serial.println(httpCode);
  }
}

// void setup()
// {
//   Serial.begin(115200);
//   delay(10);

//   // We start by connecting to a WiFi network

//   Serial.println();
//   Serial.println();
//   Serial.print("Connecting to ");
//   Serial.println(ssid);

//   WiFi.begin(ssid, password);

//   while (WiFi.status() != WL_CONNECTED)
//   {
//     delay(500);
//     Serial.print(".");
//   }

//   Serial.println("");
//   Serial.println("WiFi connected");
//   Serial.println("IP address: ");
//   Serial.println(WiFi.localIP());
// }

// void loop()
// {

//   delay(2000);

//   if ((WiFi.status() == WL_CONNECTED))
//   {
//     HTTPClient http;
//     http.begin("http://192.168.137.1:8000/");
//     int httpCode = http.GET();

//     if (httpCode > 0)
//     {
//       String payload = http.getString();
//       Serial.println("HTTP > 0");
//       Serial.println(payload);
//     }
//     else
//     {
//       Serial.println("Error with HTTP!");
//     }
//     Serial.println(httpCode);
//   }
// }
