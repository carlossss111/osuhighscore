![logo](https://external-preview.redd.it/I146DiZtDQi2yE36r6aEG9EgEkjnSjpq9gXc-OjjZGE.png?auto=webp&s=e5e27f14d519ab37ae79ccdb6da4c82f0ba98fe6 "osu!")
# Osu!Highscore
## Overview
The main function is that the Servo will rotate and hold a physical flag whenever the associated account gains PP (and therefore reaches a highscore) on the game Osu!. Targeted for the Arduino MKR1000 (other WiFi enabled boards should work fine), the code connects to the profile page api, records the score, and flags if that score is updated from the last time it has been read.
## Hardware
Main:
* Arduino MKR1000 (or any similiar wifi enabled board).
* Servo, connected to a PWD pin (5), VCC and GND. Include a 100μF capacitor in parrallel with the VCC and GND.

Optional
* TM1637 Display, connected to VCC, GND, CLK > 3, DI0 > 2. If you do not want to include this, remove the instances of display functions.
## Usage
1. Setup the hardware.
2. Download this software.
3. Rename `yourcodeshere.h` to `wificodes.h`.
4. Edit the file from the previous step. The API key and querystring can be tested by appending them to osu.ppy.sh and putting the resulting string into a browser. A JSON file should be returned.
```objectiveC
char ssid[] = "YOUR WIFI SSID HERE";
char pass[] = "YOUR WIFI PASSWORD HERE";
String apikey = "YOUR API KEY HERE";
String query = "/api/get_user?u=YOUR USERNAME HERE";
String keyIndex = 0;//<--Enter an integer only for WEP connections, see next part.
```
5. This code deals with WPA (therefore most) WiFi encryption, if you are using WEP or an open network, tweak osuhighscore.ino to replace `status = WiFi.begin(ssid, pass);` with either:
```C++
status = WiFi.begin(ssid); //FOR NO ENCRYPTION.
status = WiFi.begin(ssid, keyIndex, key); //FOR WEP ENCRYPTION.
```
6. Download the dependencies in the section below.
7. Upload the code. Check everything works using the serial monitor. Once connected to WiFi it should print out some details from your profile every few seconds.
8. Make some good Osu! plays.
## Dependencies
All of the dependencies are available for download in the Arduino IDE library manager.
* Servo (usually pre-installed) (https://www.arduino.cc/en/Reference/Servo)
* WiFi101 (https://www.arduino.cc/en/Reference/WiFi101)
* ArduinoJson (https://github.com/bblanchon/ArduinoJson)
* TM1637 Display (https://github.com/avishorp/TM1637)
## Software
The code is currently split up into three main parts.
1. Display
2. WiFi & Server
3. Servo, setup and loop.
The Display part has functions called by the WiFi and Servers part that update the display to show the appropriate score or message. If you would not like to include the display, this section and any references to it can simply be removed and the rest will work fine. The WiFi and Server part has functions called by the Setup and Loop, it is responsible for connecting to WiFi, the server and retrieving the JSON response. The final section is responsible for controlling the servo when the score changes, and also for calling everything else.
## Result
![result](./result.jpg "result")
