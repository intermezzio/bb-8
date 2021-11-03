#include <Adafruit_MotorShield.h>

// initialize motor shield
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// define motors
Adafruit_DCMotor *leftMotor = AFMS.getMotor(2);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(3);

int speed_ = 6;

struct path {
  float direction;
  int duration;
  int pace;
};

struct motorspeeds {
  int leftSpeed;
  int rightSpeed;
}

// path array
path paths[] = {
  {1, 3000, 6},
  {-1, 2000, 6},
  {1, 1000, 0}
};
int num_paths = sizeof(paths)/sizeof(paths[0]);

void setup() {
  Serial.begin(9600);
  
  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
      Serial.println("Could not find Motor Shield. Check wiring.");
    }
  }
  Serial.println("Motor Shield found.");
  
  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);
}

motorspeeds chooseSpeed(path p) {
  int pos = p.pos;
  int speed_ = p.pace;
  int leftMotorDirection = FORWARD;
  int rightMotorDirection = FORWARD;
  
  int leftMotorSpeed = 0;
  int rightMotorSpeed = 0;
  switch(pos) {
    case 1:
      leftMotorSpeed = 4;
      rightMotorSpeed = 4;
      break;
    case -1:
      leftMotorSpeed = -4;
      rightMotorSpeed = -4;
      break;
    default:
      break;
  }
  leftMotorSpeed *= speed_;
  rightMotorSpeed *= speed_;

  motorspeeds newSpeed = {leftMotorSpeed, rightMotorSpeed};
  return newSpeed;
}

void driveMotors(motorspeeds speeds) {
  int leftMotorSpeed = speeds.leftMotorSpeed;
  int rightMotorSpeed = speeds.rightMotorSpeed;
  
  Serial.print(leftMotorSpeed);
  Serial.print(",");
  Serial.println(rightMotorSpeed);

  if(leftMotorSpeed < 0) {
    leftMotorSpeed = -leftMotorSpeed;
    leftMotorDirection = BACKWARD;
  }
  if(rightMotorSpeed < 0) {
    rightMotorSpeed = -rightMotorSpeed;
    rightMotorDirection = BACKWARD;
  }

  leftMotor->run(leftMotorDirection);
  rightMotor->run(rightMotorDirection);
  
  leftMotor->setSpeed(leftMotorSpeed);
  rightMotor->setSpeed(rightMotorSpeed);
  
  return;
}

void getInput() {
  if(Serial.available() == 0) {
    return;
  }
  int newSpeed = Serial.parseInt();
  if(newSpeed < 0 || newSpeed > 32) {
    // invalid data
    Serial.print("Invalid speed ");
    Serial.println(newSpeed);
    return;
  }
  if(newSpeed == 0) {
    Serial.println("Terminating code");
    // end code by trapping in a while loop
    leftMotor->run(RELEASE);
    rightMotor->run(RELEASE);
    while(true) {}
  }
  speed_ = newSpeed;
  return;
}

void loop() {
  // for element in list of paths
  for(int i = 0; i < num_paths; i++) {
    path p = paths[i];
    motorspeeds s = chooseSpeeds(p);
    driveMotors(s);
    delay(p.duration);
  }
  leftMotor->run(RELEASE);
  rightMotor->run(RELEASE);
  while(true) {}
}
