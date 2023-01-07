#include <ESP8266WiFi.h>
#include <NTPClient.h>

const char* ssid = "your_ssid";  // replace with your WiFi SSID
const char* password = "your_password";  // replace with your WiFi password
const int transistorPin = 2;  // pin that the transistor is connected to

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void setup() {
  pinMode(transistorPin, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  timeClient.begin();
}

void loop() {
  timeClient.update();
  if (timeClient.getHour() == 19 && timeClient.getMinute() == 0 && timeClient.getSecond() == 0) {  // check if it is 7:00 PM
    digitalWrite(transistorPin, HIGH);  // turn the transistor on
    delay(20000);                       // wait for 20 seconds
    digitalWrite(transistorPin, LOW);   // turn the transistor off
  }
}