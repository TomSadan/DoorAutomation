//www.elegoo.com
//2016.12.12

/************************
Exercise the motor using
the L293D chip, Unlock door with a timer using two limit switches

Author: Tom Sadan, 2023
************************/
#include "pitches.h"

#define BUZZER 11

#define ENABLE 5
#define DIRA 3
#define DIRB 4

#define LEDPIN 6
#define FRONTLIMITSWITCH 8
#define BACKLIMITSWITCH 9
#define ACTIVATIONBUTTON 10

#define OFF 0
#define FORWARD 1
#define BACKWARD 2

#define TIME_DOOR_UNLOCKED_MS 5000

int state = OFF;


int digitalPinsCurrentState[] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
int digitalPinsLastState[] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
unsigned long lastDebounceTime[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned long debounceDelay = 50;
 
void setup() {
  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);

  pinMode(LEDPIN, OUTPUT);
  pinMode(FRONTLIMITSWITCH, INPUT_PULLUP);  
  pinMode(BACKLIMITSWITCH, INPUT_PULLUP);  
  pinMode(ACTIVATIONBUTTON, INPUT_PULLUP);  

  Serial.begin(9600);
}

//////// MOVEMENT ////////

void forward(int speed=HIGH) {
  digitalWrite(ENABLE,speed); // enable on
  digitalWrite(DIRA,HIGH); //one way
  digitalWrite(DIRB,LOW);
}

void backward(int speed=HIGH) {
  digitalWrite(ENABLE,speed); // enable on
  digitalWrite(DIRA,LOW); //one way
  digitalWrite(DIRB,HIGH);
}

void stop() {
  digitalWrite(ENABLE,LOW); // disable
}

void fastStop() {
  digitalWrite(DIRA,LOW); //fast stop
  digitalWrite(DIRB,LOW); //fast stop
  stop();
}

//////// STATES ////////

void stateOff(){
    state = OFF;
    digitalWrite(LEDPIN, LOW);
    fastStop();
}

void stateForward(){
    state = FORWARD;
    forward();
    digitalWrite(LEDPIN, HIGH);
}

void stateBackward(){
    state = BACKWARD;
    fastStop();
    delay(TIME_DOOR_UNLOCKED_MS);
    backward();
    digitalWrite(LEDPIN, HIGH);
}
//////// DEBOUNCING ////////

int debounceRead(int input) {
  int reading = digitalRead(input);
  if (reading != digitalPinsLastState[input]) {
    lastDebounceTime[input] = millis();
  }

  else if ((millis() - lastDebounceTime[input]) > debounceDelay) {
    digitalPinsCurrentState[input] = reading;
  }
  digitalPinsLastState[input] = reading;
  return digitalPinsCurrentState[input];
}

//////// MUSIC ////////

void MickieSong() {
  tone(BUZZER, NOTE_D5, 200);
  delay(200);
  tone(BUZZER, NOTE_D5, 200);
  delay(200);
  tone(BUZZER, NOTE_D5, 200);
  delay(500);
  tone(BUZZER, NOTE_D5, 200);
  delay(200);
  tone(BUZZER, NOTE_D5, 200);
  delay(200);
  tone(BUZZER, NOTE_D5, 200);
  delay(500);
  tone(BUZZER, NOTE_D5, 200);
  delay(200);
  tone(BUZZER, NOTE_D5, 200);
  delay(200);
  tone(BUZZER, NOTE_D5, 200);
  delay(500);
  tone(BUZZER, NOTE_C5, 200);
  delay(200);
  tone(BUZZER, NOTE_C5, 200);
  delay(200);
  tone(BUZZER, NOTE_C5, 200);
}

void OpeningSound() {
  tone(BUZZER, NOTE_G5, 300);
  delay(250);
  tone(BUZZER, NOTE_C5, 700);
}

//////// MAIN CODE ////////

void loop() {  
  if (digitalRead(BACKLIMITSWITCH) == LOW && state == BACKWARD)
  {
    stateOff();
    MickieSong();
  }
  if (digitalRead(FRONTLIMITSWITCH) == LOW && state == FORWARD) {
    stateBackward();
  }
  if (debounceRead(ACTIVATIONBUTTON) == LOW && state == OFF) {
    OpeningSound();
    stateForward();
  }
}
   
