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

//Include JSON parser.
#include <ArduinoJson.h>

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
}

//Loads the page and stores the output in memory, for reading in checkForNewScore().
void loadPage(){
  char host[] = "osu.ppy.sh";
  String query = "/api/get_user?u=carlossss111";
  String apikey = "k=d053af8f95b3d2d8e6dbd502424b9b009798fbc2";
  String endpoint = query + "&" + apikey;
  //https://osu.ppy.sh/api/get_user?u=carlossss111&k=d053af8f95b3d2d8e6dbd502424b9b009798fbc2
  Serial.println("Connecting to: " + String(host) + endpoint + "...");
  //On a connection, make the request and construct the header.
  if (client.connect(host, 80)) {
    Serial.println("Connected to server!");
    client.println("GET " + endpoint + " HTTP/1.1");
    client.println("Host: osu.ppy.sh");
    client.println("Connection: close");
    client.println();
  }
}


//Stores new pp value each time it increases and uses it for comparison.
int storedpp = 0;
//Checks the response of the HTTP request in loadPage().
//Must be in the loop() function.
bool checkForNewScore(){
  //Reads the entire response into the rawOutput string.
  String rawOutput = "";
  if (client.available()){
    rawOutput = client.readString();
  }

  //Selects the {...} part of the output, which would be the JSON file.
  String JSONresponse = rawOutput.substring(rawOutput.indexOf("{"), rawOutput.indexOf("}") + 1);
  if(JSONresponse != ""){
    Serial.println("Raw JSON Response: " + JSONresponse);
  }

  //Using #included arduinoJson, creates static JSON object called 'doc'.
  StaticJsonDocument<1024> doc; 

  //Assigns the JSON object doc as the deserialised JSON from JSONresponse.
  DeserializationError err = deserializeJson(doc, JSONresponse);
  //The incompleteInput error occors while connecting. Otherwise the error is problematic.
  if (err) {
    if (err.c_str() == "IncompleteInput"){
      Serial.println("Recieving JSON...");
    }
    else{
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(err.c_str());
    }
  }

  //The score, "pp", is loaded into a variable using the arduinoJson syntax.
  int newpp = doc["pp_raw"].as<int>();
  //If the pp value has been properly got from the host.
  if (newpp > 1){
    Serial.println("Username: " + doc["username"].as<String>() + "\nPP rank: " + doc["pp_rank"].as<int>());
    //If stored pp is zero, the device has just been turned on.
    if (storedpp == 0){
      storedpp = newpp;//Load the intial pp value.
      Serial.println("Stored pp initialised to: " + String(storedpp));
    }
    else if (storedpp < newpp){
      Serial.println("HIGHSCORE!!!\npp previously: " + String(storedpp) + "\npp now: " + String(newpp));
      storedpp =  newpp;//Loads new pp value.
      return true;//return true so that the servo can be run from the loop() function.
    }
  }

  //If the client is disconnected, end the client object.
  if (!client.connected()) {
    Serial.println("Disconnected to server.");
    client.stop();
  }

  delay(1000);
  return false;
}