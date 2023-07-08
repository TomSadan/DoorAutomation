//www.elegoo.com
//2016.12.12

/************************
Exercise the motor using
the L293D chip, Unlock door with a timer using two limit switches

Author: Tom Sadan, 2023
************************/

#define ENABLE 5
#define DIRA 3
#define DIRB 4

#define LEDPIN 6
#define BUTTONFORWARD 8
#define BUTTONBACKWARD 9
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
  pinMode(BUTTONFORWARD, INPUT_PULLUP);  
  pinMode(BUTTONBACKWARD, INPUT_PULLUP);  
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

void loop() {  
  if (digitalRead(BUTTONBACKWARD) == LOW && state == BACKWARD)
  {
    stateOff();
  }
  if (digitalRead(BUTTONFORWARD) == LOW && state == FORWARD) {
    stateBackward();
  }
  if (debounceRead(ACTIVATIONBUTTON) == LOW && state == OFF) {
    stateForward();
  }
}
   
