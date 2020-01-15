/* Bill, Sheng, Steven
 * Mr. Wong
 * 2020-01-15
 * This program controls an Arduino to simulate a real-life T-Intesection.
 */

//Libraries Used
#include<Servo.h>

//Variable Declaration
// Variable declarations for the two perpendicular traffic lights. (Adjacent lights are connected in parallel, so no extra variable is needed)
int redTrafficLight1 = 10;
int yellowTrafficLight1 = 9;
int greenTrafficLight1 = 8;
int redTrafficLight2 = 13;
int yellowTrafficLight2 = 12;
int greenTrafficLight2 = 11;
// Variable declarations for the pedestrian lights and button.
int greenPedestrianLight = 6;
int redPedstrianLight = 7;
int pedestrianButton = 2;
// Variable declaration for the button state
int buttonState = LOW;
// Variable declaration for light detected by photoresistor.
int light = 0;
// Variable declaration for previous and current system time - to control delays.
unsigned long previousTime = 0;
unsigned long currentTime = 0;
// Variable delcaration for state of traffic lights
int currentTrafficState = 0;
// Variable declaration for servo to control parking gates.
Servo servo;
// Variable declaration for traffic timings.
int state0Time = 4000;
int state1Time = 2000;
int state2Time = 750;
int state3Time = 4000;
int state4Time = 2000;
int state5Time = 750;

//Setup - Called at program start to initialize and setup arduino.
void setup()
{
  // Setup for pinmodes of traffic/pedestrian lights. 
  pinMode(redTrafficLight1, OUTPUT);
  pinMode(yellowTrafficLight1, OUTPUT);
  pinMode(greenTrafficLight1, OUTPUT);
  pinMode(redTrafficLight2, OUTPUT);
  pinMode(yellowTrafficLight2, OUTPUT);
  pinMode(greenTrafficLight2, OUTPUT);
  pinMode(greenPedestrianLight, OUTPUT);
  pinMode(redPedstrianLight, OUTPUT);
  pinMode(pedestrianButton, INPUT);

  // Attach servo to pin 4
  servo.attach(4);
  // Initialize servo's initial position to closed.
  servo.write(90);
}

//Loop - Main program where all methods are run in an infinite loop.
void loop()
{
  // Read button state (On or Off)
  buttonState = digitalRead(1);
  // Read photoresistor state (How much light detected)
  light = analogRead(A0);
  // Stores current system time. 
  currentTime = millis();
  // Checks if button is pressed
  if(buttonState == HIGH){
    buttonPressed(); // Calls buttonPressed() method
  }
  controlStreetLights(); // Controls street lights
  controlServo(); // Controls servo
  changeLights(); // Changes traffic lights
}

/**
 * This method changes the traffic lights, using system time as a delay. 
 */
void changeLights(){
  if(currentTrafficState == 0){
    //Red, Green
    digitalWrite(redTrafficLight2, LOW);
    digitalWrite(redPedstrianLight, LOW);
    digitalWrite(redTrafficLight1, HIGH);
    digitalWrite(greenTrafficLight2, HIGH);
    digitalWrite(greenPedestrianLight, HIGH);
    asyncDelay(state0Time); // Delay 4s
  }
  else if(currentTrafficState == 1){
    //Red, Yellow
    digitalWrite(greenTrafficLight2, LOW);
    digitalWrite(yellowTrafficLight2, HIGH);
    asyncDelay(state1Time); // Delay 2s
  }
  else if(currentTrafficState == 2){
    //Red, Red
    digitalWrite(yellowTrafficLight2, LOW);
    digitalWrite(greenPedestrianLight, LOW);
    digitalWrite(redTrafficLight2, HIGH);
    digitalWrite(redPedstrianLight, HIGH);
    asyncDelay(state2Time); // Delay .75s
  }
  else if(currentTrafficState == 3){
    //Green, Red
    digitalWrite(redTrafficLight1, LOW);
    digitalWrite(greenTrafficLight1, HIGH);
    asyncDelay(state3Time); // Delay 4s
  }
  else if(currentTrafficState == 4){
    //Yellow, Red
    digitalWrite(greenTrafficLight1, LOW);
    digitalWrite(redTrafficLight1, LOW);
    digitalWrite(yellowTrafficLight1, HIGH);
    asyncDelay(state4Time); // Delay 2s
  }
  else if(currentTrafficState == 5){
    // Reset traffic light times.
    state3Time = 4000;
    state4Time = 2000;
    state5Time = 750;
    //Red, Red
    digitalWrite(yellowTrafficLight1, LOW);
    digitalWrite(redTrafficLight1, HIGH);
    asyncDelay(state5Time); // delay .75s
  }
}

/**
 * This method is called when a button is pressed. It controls which lights' timings
 *  are changed based on current traffic state.
 */
void buttonPressed(){
  // As specified in the assignment sheet, when the button is pressed, the timing of the T-intersection yellow and red LED will be 
  //  cut in half. This will allow the pedestrian wait less to cross.
  
  // If pressed while T-intersection light is green, next cycle is halved.
  if(currentTrafficState == 3 && state3Time != 2000){
    state4Time = 1000;
  }
  // If pressed while T-intersection light is yellow, time is halved.
  if(currentTrafficState == 4 && state4Time != 1000){
    previousTime -= (currentTime - previousTime)/2;
  }else{
    //If pressed while pedestrian crossing light is green, T-intersection lights will be halved when they occur after.
    //if(currentTrafficState == 0 || currentTrafficState == 1 || currentTrafficState == 2){
      state3Time = 2000;
      state4Time = 1000;
    //}
  }
  buttonState = LOW;
}

/**
 * This method controls the street lights based on light detected by the photoresistor.
 */
void controlStreetLights(){
  if(light < 10){
    digitalWrite(3, HIGH); 
  }else{
    digitalWrite(3, LOW);
  }
}

/**
 * This method controls the parking gate's servo. It changes in
 *  accordance to the traffic lights as specified in the assignment sheet.
 */
void controlServo(){
  if(currentTrafficState == 0){
    servo.write(20);
  }else{
    servo.write(90);
  }
}

/**
 * This method serves as an asynchronous alternative to the delay() method. It
 *  relies on system time to calculate delays.
 *  
 *  @param delayTime - The time to delay for.
 */
void asyncDelay(int delayTime){
  if(currentTime - previousTime >= delayTime){
    currentTrafficState++; 
    previousTime = currentTime;
    if(currentTrafficState == 6){
      currentTrafficState = 0;
    }
  }
}
