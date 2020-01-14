//Libraries Used
#include<Servo.h>


//Variable Declaration
int red1 = 10;
int yellow1 = 9;
int green1 = 8;
int red2 = 13;
int yellow2 = 12;
int green2 = 11;
int pedGreen = 6;
int pedRed = 7;
int debug = 3;

int button1 = 2;
int buttonState = LOW;

int light = 0;

int pin = A5;

unsigned long previousTime = 0;
unsigned long currentTime = 0;
unsigned long servoPreviousTime = 0;

int currentState = 0;

int IR = A1;
int sensorValue = 0;
int outputValue = 0;

Servo servo;

void setup()
{
  Serial.begin(9600);
  
  pinMode(red1, OUTPUT);
  pinMode(yellow1, OUTPUT);
  pinMode(green1, OUTPUT);
  pinMode(red2, OUTPUT);
  pinMode(yellow2, OUTPUT);
  pinMode(green2, OUTPUT);
  pinMode(pedGreen, OUTPUT);
  pinMode(pedRed, OUTPUT);
  pinMode(debug, OUTPUT);
  
  pinMode(IR, INPUT);
  
  pinMode(button1, INPUT);

  Serial.begin(9600);
  servo.attach(4);
  servo.write(90);
}

void loop()
{

  sensorValue = analogRead(pin);
  Serial.print("\nSensor: ");
  Serial.print(sensorValue);
  if(sensorValue < 1000){
    servoPreviousTime = currentTime;
    Serial.print("\nOBJECT DETECTED: ");
    Serial.print(sensorValue);
    servo.write(180);
  }else{
    if(currentTime - servoPreviousTime >= 6000){
      servo.write(90);
    }
  }

  buttonState = digitalRead(1);
  light = analogRead(A0);
  Serial.println(light);
  Serial.println(buttonState);
  
  if(light < 5){
    digitalWrite(3, HIGH); 
  }else{
    digitalWrite(3, LOW);
  }
  
  if(buttonState == HIGH){
    if(currentState == 3){
      currentState++;
      previousTime = currentTime;
      buttonState = LOW;
    }
  }

  //Using millis() instead of delay(), so program doesn't pause completely.
  currentTime = millis();
  changeLights();
  delayMicroseconds(1000);
}

void changeLights(){
  if(currentState == 0){
    //Red, Green
    digitalWrite(red2, LOW);
    digitalWrite(pedRed, LOW);
    digitalWrite(red1, HIGH);
    digitalWrite(green2, HIGH);
    digitalWrite(pedGreen, HIGH);
    asyncDelay(4000);
  }
  else if(currentState == 1){
    //Red, Yellow
    digitalWrite(green2, LOW);
    digitalWrite(yellow2, HIGH);
    asyncDelay(2000);
  }
  else if(currentState == 2){
    //Red, Red
    digitalWrite(yellow2, LOW);
    digitalWrite(pedGreen, LOW);
    digitalWrite(red2, HIGH);
    digitalWrite(pedRed, HIGH);
    asyncDelay(750);
  }
  else if(currentState == 3){
    //Green, Red
    digitalWrite(red1, LOW);
    digitalWrite(green1, HIGH);
    asyncDelay(4000);
  }
  else if(currentState == 4){
    //Yellow, Red
    digitalWrite(green1, LOW);
    digitalWrite(red1, LOW);
    digitalWrite(yellow1, HIGH);
    asyncDelay(2000);
  }
  else if(currentState == 5){
    //Red, Red
    digitalWrite(yellow1, LOW);
    digitalWrite(red1, HIGH);
    asyncDelay(750);
  }
}

void asyncDelay(int delayTime){
  if(currentTime - previousTime >= delayTime){
    currentState++; 
    previousTime = currentTime;
    if(currentState == 6){
      currentState = 0;
    }
  }
}
