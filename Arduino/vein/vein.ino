#include <AccelStepper.h>

#define STEP_PIN 8
#define DIR_PIN 9
#define EN_PIN 10

AccelStepper vein(AccelStepper::DRIVER, STEP_PIN, DIR_PIN); //create driver object

const long STEP_PER_REVOLUTION = 200; // define this based on your stepper motor's specifications

unsigned long previousMillis = 0; // stores last time update

// State machine for the motor's direction and duration, names reference systole and diastole
enum MotorState {
  sys_rise,
  sys_fall,
  dia_rise,
  dia_fall
};

MotorState currentMotorState = sys_rise;

void setup() {
  vein.setMaxSpeed(250); // Set max speed as position to travel / time to travel
  vein.setAcceleration(100);
  vein.move(-250);
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);
}

void loop() {
  unsigned long currentMillis = millis();

  switch (currentMotorState) {
    case sys_rise:
      if (currentMillis - previousMillis >= 2500) {
        vein.setMaxSpeed(125); // Change max speed 
        vein.move(125); // Change direction to CCW
        currentMotorState = sys_fall;
        previousMillis = currentMillis; // Remember the switch time
      }
      break;

    case sys_fall:
      if (currentMillis - previousMillis >= 2500) {
        vein.setMaxSpeed(25); // Change max speed 
        vein.move(-25); // Change direction to CW
        currentMotorState = dia_rise;
        previousMillis = currentMillis;
      }
      break;

    case dia_rise:
      if (currentMillis - previousMillis >= 2000) {
        vein.setMaxSpeed(150); // Change max speed 
        vein.move(150); // Change direction to CCW
        currentMotorState = dia_fall;
        previousMillis = currentMillis;
      }
      break;

    case dia_fall:
      if (currentMillis - previousMillis >= 2500) {
        vein.setMaxSpeed(250); // Change max speed 
        vein.move(-250); // Change direction to CW
        currentMotorState = sys_rise; // Loop back to the first state
        previousMillis = currentMillis;
      }
      break;
  }

  vein.run();
}
