#include <Servo.h>

#define AXIS 2

int stepperPins[AXIS]         = {6, 2};
int dirPins[AXIS]             = {7, 3};
int stopPins[2*AXIS]          = {10, 11, 13, 12};

int cncCurrentState[AXIS+1]   = {0};
int cncNextState[AXIS+2]      = {0};
int maxSteps[AXIS]            = {0};

Servo zServo;  // create servo object to control a servo

String strIn = "";

void setup() {
  Serial.begin(9600);
  
  for(int i=0; i<AXIS; ++i){
    pinMode(stepperPins[i], OUTPUT);
    pinMode(dirPins[i], OUTPUT);
    pinMode(stopPins[2*i], INPUT);
    pinMode(stopPins[2*i+1], INPUT);
  }

  zServo.attach(44);
  resetServo();
  
  calibrate();
  justMove();
  Serial.println("SystemReady");
}

void resetServo(){
  //Serial.println("Seting up servo initial position");
  zServo.write(0);
  delay(1000);
  //Serial.println("Servo to 90");
  zServo.write(90);
  delay(1000);
  //Serial.println("Servo to 0");
}

void calibrate(){
  //Serial.println("Calibrating steppers");
  //Serial.print("Max steps per axis: ");
  for(int i=0;  i<AXIS; ++i){
    while(!digitalRead(stopPins[2*i]))step(stepperPins[i], dirPins[i], 0, 1);
    while(!digitalRead(stopPins[2*i+1])){
      step(stepperPins[i], dirPins[i], 1, 1);
      maxSteps[i]++;
    }
    cncCurrentState[i+1] = maxSteps[i];
    //Serial.print(maxSteps[i]);
    //Serial.print(" ");
  }
  //Serial.println("");
}

void step(int stepperPin, int dirPin, boolean dir,int steps){
  digitalWrite(dirPin,dir);
  delayMicroseconds(200);
  for(int i=0;i<steps;i++){
    digitalWrite(stepperPin, HIGH);
    delayMicroseconds(200);
    digitalWrite(stepperPin, LOW);
    delayMicroseconds(200);
  }
}

void step(int *dirs, int *steps){
  int totalSteps = 0;
  for(int i=0;  i<AXIS; ++i){
    digitalWrite(dirPins[i],dirs[i]);
    totalSteps += steps[i];
  }
  delayMicroseconds(200);
  while(totalSteps>0){
    for(int i=0;  i<AXIS; ++i){
      if(steps[i]>0){
        digitalWrite(stepperPins[i], HIGH);
        delayMicroseconds(200);
        digitalWrite(stepperPins[i], LOW);
        delayMicroseconds(200);
        steps[i]--;
        totalSteps--;
      }
      
    }
  }
}

bool stringFromSerial() {
  while (Serial.available()) {
    char c = Serial.read();
    strIn += c;
    if (c == '\n'){
      return true;
    }
  }
  return false;
}

int parseInput(){
  if (stringFromSerial()) {
    //Serial.print("Got Input: ");
    //Serial.println(strIn);
    int i = 0;
    int j = 0;
    String str = "";
    while (strIn[i] != '\n' && i<=AXIS*20){
      if (strIn[i] == ':') {
        cncNextState[j] = str.toInt();
        str = "";
        i++;
        j++;
      }
      str += strIn[i];
      i++;
    }
    strIn = "";
    return 1;
  }
  return 0;
}

void justMove(){
  int steps = 0;
  bool dir   = 0;
  int dirs[AXIS] = {0};
  int stepArray[AXIS] = {0};
  
  if(cncNextState[0]==1)zServo.write(0);
  else zServo.write(90);
  if(cncNextState[0] != cncCurrentState[0])delay(500);
  cncCurrentState[0] = cncNextState[0];
  
  for (int i=1; i<=AXIS; ++i){
    steps = cncNextState[i] - cncCurrentState[i];
    dir = steps >= 0;
    steps = min(maxSteps[i-1], abs(steps));
    /*Serial.print("Movin' stepper: ");
    Serial.print(i-1);
    Serial.print(" ");
    Serial.print(steps);
    Serial.println(" steps");*/
    dirs[i-1] = dir;
    stepArray[i-1] = steps;
    cncCurrentState[i] = cncNextState[i];
  }
  step(dirs, stepArray);
}

void circle(){
  int r = cncNextState[AXIS+1];
  int p = cncNextState[1];
  int q = cncNextState[2];
  float t = 0;
  
  cncNextState[0] = 0;
  cncNextState[1] = r+p;
  justMove();

  cncNextState[0] = 1;
  while(t<=TWO_PI+HALF_PI){
    cncNextState[1] = (int)(r*cos(t)+p);
    cncNextState[2] = (int)(r*sin(t)+q);
    justMove();
    t+=0.25;
  }
  zServo.write(90);
  delay(16);
  cncNextState[AXIS+1]=0;
}

void loop(){
  if(parseInput()){
    if(cncNextState[AXIS+1]==0)justMove();
    else circle();
    Serial.println("SystemReady");
  }
}
