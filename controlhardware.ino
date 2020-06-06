//This file is secondary, it is called second and after the main.

//servo library
#include <Servo.h>

//#include "osuhighscore.ino";//This line is just to please VScode/

//pins
//LED pin defined previously
#define BUTTONpin 0
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
  pinMode(LEDpin, OUTPUT);
  pinMode(BUTTONpin, INPUT);

  WiFiSetup();
  loadPage();
}

//Called constantly.
void loop() {
  //Check if button is pressed.
  if(digitalRead(BUTTONpin) == HIGH){
    loadPage();
  }
  //If connected (after loadPage()) then check the response.
  if (client.connected()){
    checkForNewScore();
  }
  //if(checkForNewScore()){
  //  loadServo(90, 2000);
  //}
}