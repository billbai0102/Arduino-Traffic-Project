#include<Servo.h>
int pin = A5;
int sensorValue = 0;
Servo servo;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servo.attach(4);
  servo.write(90);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorValue = analogRead(pin);
  Serial.print("\nSensor: ");
  Serial.print(sensorValue);
  if(sensorValue < 1010){
    Serial.print("\nOBJECT DETECTED: ");
    Serial.print(sensorValue);
    servo.write(180);
  }else{
    servo.write(90);
  }
}
