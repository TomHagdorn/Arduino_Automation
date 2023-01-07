#include <ESP8266WiFi.h>
#include <NTPClient.h>

const char* ssid = "your_ssid";  // replace with your WiFi SSID
const char* password = "your_password";  // replace with your WiFi password
const int transistorPin = 2;  // pin that the transistor is connected to

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

int lastActivationTime = 0;  // store the last time the transistor was activated

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
  int currentTime = timeClient.getEpochTime();  // get the current time in epoch format
  if (timeClient.getHour() == 19 && timeClient.getMinute() == 0 && timeClient.getSecond() == 0 && 
      currentTime - lastActivationTime >= 3 * 24 * 60 * 60) {  // check if it is 7:00 PM and at least 3 days have passed since the last activation
    digitalWrite(transistorPin, HIGH);  // turn the transistor on
    delay(20000);                       // wait for 20 seconds
    digitalWrite(transistorPin, LOW);   // turn the transistor off
    lastActivationTime = currentTime;  // update the last activation time
  }
}