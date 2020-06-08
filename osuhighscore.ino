/*FIRST PART OF INSTRUCTION
* The pp is output onto a display.
* displaySetup() is called by setup() sets up the display.
* displayScore() is called by checkForNewScore() and displays the score.
*/
#include <TM1637Display.h>
#define CLK 3
#define DIO 2
TM1637Display display(CLK, DIO);

//On setup, have the display say "load"ing.
void displaySetup(){

  //Segments for loading.
  const uint8_t loading[] = {
  SEG_F | SEG_E | SEG_D,                           // L
  SEG_G | SEG_C | SEG_D | SEG_E ,                  // o
  SEG_E | SEG_F | SEG_G | SEG_B | SEG_C | SEG_A,   // A
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
  };

  display.clear();
  display.setBrightness(7);//0-7
  display.setSegments(loading);
}

//Called each time there is a new score.
void displayScore(int score){
  display.showNumberDec(score);
}

//Displays an error message
void displayError(){

  //Segments for errors.
  const uint8_t error[] = {
  SEG_E | SEG_F | SEG_A | SEG_G | SEG_D,    // E
  SEG_E | SEG_G,                            // r
  SEG_E | SEG_G,                            // r
  SEG_D,                                    // _
  };

  display.setSegments(error);
  //More error handling WIP...
}

/*SECOND PART OF INSTRUCTION
* Everything in this part deals with connecting to WiFI and the server.
* Wificodes.h is required.
* WifiSetup() is called from setup(), it connects to a WPA network.
* loadPage() is called every 4 seconds (while the not already connected to the server). It connects to the server.
* checkForNewScore() is called whenever the loadPage() function has successfully connected to the server,
*    -it requests the output from the server, parses it and returns true only if the score has changed from last time.
*/

//Wifi101 library. Might not need all of these but here for safe measure.
#include <WiFi101.h>
#include <WiFiClient.h>

//Include JSON parser.
#include <ArduinoJson.h>

//WiFi settings
#include "wificodes.h"
int status = WL_IDLE_STATUS;     // the WiFi radio's status
WiFiClient client; //initialise client object.

void WiFiSetup(){
  //Initialises the serial port.
  Serial.begin(9600);//If there is no serial monitor connected, it doesn't matter, the messages go nowhere.

  //Attempts to connect to WPA WiFi. Repeats until successful.
  while ( status != WL_CONNECTED) {
    Serial.println("Attempting to connect to SSID: " + String(ssid) + "...");
    //Connects using ssid and password provided in a seperate file not commited, wificodes.h.
    status = WiFi.begin(ssid, pass);

    //Wait 5 seconds for connection:
    delay(5000);
  }
  //This point forward the device is connected to WiFi.
  Serial.println("Successfully connected!");
}

//Loads the page and stores the output in memory, for reading in checkForNewScore().
void loadPage(){
  char host[] = "osu.ppy.sh";
  //querystring provided in wificodes.h
  //apikey provided in wificodes.h
  String endpoint = query + "&" + apikey;
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
  //Anything in events[] is cut so that there is no memory leak.
  String JSONresponse = rawOutput.substring(rawOutput.indexOf("{"), rawOutput.indexOf("[",rawOutput.indexOf("{")) + 1);
  if(JSONresponse != ""){
    JSONresponse += "]}";//The string has been cut short, it is ended to complete the JSON.
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
      displayError();
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

    //Load display
    displayScore(newpp);
  }

  //If the client is disconnected, end the client object.
  if (!client.connected()) {
    Serial.println("Disconnected to server.");
    client.stop();
  }

  delay(1000);
  return false;
}


/*THIRD PART OF THE INSTRUCTION
* This part has the servo function, aswell as the setup and loop functions that call all the other functions.
* loadServo(angle, duration) is called whenever checkForNewScore() is true, it turns the servo.
* setup() is called on startup, it calls the WifiSetup() (and secondarily loadPage() for the first time).
* loop() is called repeatedly, it loads the page if not loaded already, and checks for the score if the already loaded.
*/

//servo library
#include <Servo.h>

//pins
//LED pin defined previously
#define SERVOpin 5

//servo object initialised
Servo myServo;

//Rotates the servo to a position for a duration of time, then rotates back to 0.
void loadServo(int angle, int duration){
  myServo.attach(SERVOpin);
  myServo.write(angle);
  delay(1200);//Delay while the servo is rotating.
  delay(duration);//Holds position for a duration.
  myServo.write(0);
  delay(1200);//Delay again while the servo is rotating.
  myServo.detach();
}

//Called on startup.
void setup() {
  displaySetup();
  WiFiSetup();
  loadPage();
}

//Called constantly.
void loop() {
  //If connected (after loadPage()) then check the response.
  if (client.connected()){
    //If the score is a new highscore, it will return true.
    if(checkForNewScore() == true){
      loadServo(90, 8000);
    }
  }
  else{
    //Check the page every 4 seconds!
    delay(4000);
    loadPage();
  }
}