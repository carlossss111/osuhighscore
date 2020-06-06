//servo library
#include <Servo.h>

//pins
#define LEDpin 3
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

//Blinks depending on the status.
//0 = nothing of note is happening.
//1 = the button has been pressed, one blink.
//2 = a high score has been registered, two blinks.
//3 = page failed to load, three blinks.
void LED(int statuscode, int duration){
  //Blinks the LED for a number of times depending on the status code.
  for(int i = 0; i < statuscode; i++){
    digitalWrite(LEDpin, HIGH);
    delay(duration);
    digitalWrite(LEDpin, LOW);
    //If there is more, delay before restarting sequence.
    if(statuscode != i){
      delay(duration);
    }
  }
}

//Called on startup.
void setup() {
  pinMode(LEDpin, OUTPUT);
  pinMode(BUTTONpin, INPUT);
}

//Called constantly.
void loop() {
  if(digitalRead(BUTTONpin) == HIGH){
    LED(1, 1000);
    loadServo(90, 2000);
  }
}
