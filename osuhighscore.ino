//This file is the main, it is called first.
//The setup and loop functions are in controlhardware.ino.

//For status updates
#define LEDpin 3

//Wifi101 library. Might not need all of these but here for safe measure.
#include <WiFi101.h>
#include <WiFiClient.h>
#include <WiFiMDNSResponder.h>
#include <WiFiServer.h>
#include <WiFiSSLClient.h>
#include <WiFiUdp.h>

//WiFi settings
#include "wificodes.h"
int status = WL_IDLE_STATUS;     // the WiFi radio's status
WiFiClient client; //initialise client object.

void WiFiSetup(){

  //LED lights until connection to WiFi is established.
  digitalWrite(LEDpin, HIGH);

  // Initialises the serial port and waits for it.
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  //Attempts to connect to WPA WiFi. Repeats until successful.
  while ( status != WL_CONNECTED) {
    Serial.println("Attempting to connect to WPA SSID: " + String(ssid) + "...");
    //Connects using ssid and password provided in a seperate file not commited, wificodes.h.
    status = WiFi.begin(ssid, pass);

    //Wait 5 seconds for connection:
    delay(5000);
  }
  //This point forward the device is connected to WiFi.

  digitalWrite(LEDpin, LOW);
  Serial.println("Successfully connected!");

  char host[] = "osu.ppy.sh";
  String query = "/api/get_user?u=carlossss111";
  String apikey = "k=d053af8f95b3d2d8e6dbd502424b9b009798fbc2";
  String endpoint = query + "&" + apikey;
  //"/api/get_user?u=carlossss111&k=d053af8f95b3d2d8e6dbd502424b9b009798fbc2";
  Serial.println("Connecting to: " + String(host) + endpoint + "...");
  // if you get a connection, report back via serial:
  if (client.connect(host, 80)) {
    Serial.println("Connected to server");
    // Make a HTTP request:
    client.println("GET " + endpoint + " HTTP/1.1");
    client.println("Host: osu.ppy.sh");
    client.println("Connection: keep-alive");
    client.println();
  }
}

//Checks site for new score.
bool checkForNewScore(){
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();
    while(true);
  }
  return false;
}

//https://osu.ppy.sh/api/get_user?u=carlossss111&k=d053af8f95b3d2d8e6dbd502424b9b009798fbc2