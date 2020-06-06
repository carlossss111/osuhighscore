//This file is the main, it is called first.
//The setup and loop functions are in controlhardware.ino.

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

void WiFiSetup(){
  // Initialises the serial port and waits for it.
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  //Attempts to connect to WPA WiFi. Repeats until successful.
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    //Connects using ssid and password provided in a seperate file not commited, wificodes.h.
    status = WiFi.begin(ssid, pass);

    //Wait 5 seconds for connection:
    delay(5000);
  }
  //This point forward the device is connected to WiFi.

  Serial.println("You're connected to the network");
}

bool checkForNewScore(){
  Serial.println(WiFi.ping("www.google.com"));
  delay(5000);
}