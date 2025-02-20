#include <AccelStepper.h>

#define STEP_PIN1 4
#define DIR_PIN1 5
#define EN_PIN1 6

#define STEP_PIN2 8
#define DIR_PIN2 9
#define EN_PIN2 10

// Stepper motor 1 setup
AccelStepper artery(AccelStepper::DRIVER, STEP_PIN1, DIR_PIN1); //create driver object
unsigned long previousMillis1 = 0;

// Stepper motor 2 setup
AccelStepper vein(AccelStepper::DRIVER, STEP_PIN2, DIR_PIN2); //create driver object
unsigned long previousMillis2 = 0;

const long STEP_PER_REVOLUTION = 200; // define this based on your stepper motor's specifications

// State machine for the motor's direction and duration
enum HeartState {
  sys_rise,
  sys_fall,
  dia_rise,
  dia_fall
};

HeartState currentHeartState = sys_rise;

void setup() {
  // Setup for artery motor
  artery.setMaxSpeed(250); // Set max speed as position to travel / time to travel
  artery.setAcceleration(100);
  artery.move(250);
  pinMode(EN_PIN1, OUTPUT);
  digitalWrite(EN_PIN1, LOW);

  // Setup for stepper 2
  vein.setMaxSpeed(250); // Set max speed as position to travel / time to travel
  vein.setAcceleration(100);
  vein.move(-250);
  pinMode(EN_PIN2, OUTPUT);
  digitalWrite(EN_PIN2, LOW);
}

void loop() {
  unsigned long currentMillis1 = millis();

  // Logic for stepper 1
  switch (currentHeartState) {
    case sys_rise:
      if (currentMillis1 - previousMillis1 >= 2500) {
        artery.setMaxSpeed(125); // Change max speed 
        artery.move(-125); // Change direction to CCW
        currentHeartState = sys_fall;
        previousMillis1 = currentMillis1;
      }
      break;

    case sys_fall:
      if (currentMillis1 - previousMillis1 >= 2500) {
        artery.setMaxSpeed(25); // Change max speed 
        artery.move(25); // Change direction to CW
        currentHeartState = dia_rise;
        previousMillis1 = currentMillis1;
      }
      break;

      case dia_rise:
      if (currentMillis1 - previousMillis1 >= 2000) {
        artery.setMaxSpeed(150); // Change max speed 
        artery.move(-150); // Change direction to CW
        currentHeartState = dia_fall;
        previousMillis1 = currentMillis1;
      }
      break;

      case dia_fall:
      if (currentMillis1 - previousMillis1 >= 2500) {
        artery.setMaxSpeed(250); // Change max speed 
        artery.move(250); // Change direction to CW
        currentHeartState = sys_rise;
        previousMillis1 = currentMillis1;
      }
      break;
  }

  unsigned long currentMillis2 = millis();

  // Logic for stepper 2
  switch (currentHeartState) {
    case sys_rise:
      if (currentMillis2 - previousMillis2 >= 2500) {
        vein.setMaxSpeed(125); // Change max speed 
        vein.move(125); // Change direction to CCW
        currentHeartState = sys_fall;
        previousMillis2 = currentMillis2;
      }
      break;

    case sys_fall:
      if (currentMillis2 - previousMillis2 >= 2500) {
        vein.setMaxSpeed(25); // Change max speed 
        vein.move(-25); // Change direction to CW
        currentHeartState = dia_rise;
        previousMillis2 = currentMillis2;
      }
      break;

      case dia_rise:
      if (currentMillis2 - previousMillis2 >= 2000) {
        vein.setMaxSpeed(150); // Change max speed 
        vein.move(150); // Change direction to CW
        currentHeartState = dia_fall;
        previousMillis2 = currentMillis2;
      }
      break;

      case dia_fall:
      if (currentMillis2 - previousMillis2 >= 2500) {
        vein.setMaxSpeed(250); // Change max speed 
        vein.move(-250); // Change direction to CW
        currentHeartState = sys_rise;
        previousMillis2 = currentMillis2;
      }
      break;
  }

  // Run the steppers at the set speeds
  artery.run();
  vein.run();
}
