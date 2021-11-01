#include <Adafruit_MotorShield.h>

// initialize motor shield
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// define motors
Adafruit_DCMotor *leftMotor = AFMS.getMotor(2);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(3);

// define pins
const int leftIRPin = A0;
const int rightIRPin = A1;

int speed_ = 6;

struct path {
  float direction;
  int duration;
  int pace;
};

// path array
path path1 = {1, 3000, 6};
//path path2 =;
path paths[] = {
  path1,
  {-1, 2000, 6}
};
int num_paths = sizeof(paths)/sizeof(paths[0]);

void setup() {
  pinMode(leftIRPin, INPUT);
  pinMode(rightIRPin, INPUT);

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
char processIRData(int leftIR, int rightIR) {
  bool leftDetect = true;
  bool rightDetect = true;
  if(!leftDetect && !rightDetect) {
    return 'g';
  } else if(rightDetect && !leftDetect) {
    // correct by moving right
    return 'l';
  } else if(leftDetect && !rightDetect) {
    // correct by moving left
    return 'r';
  } else {
    // should not reach this state
    // both see the tape
    return 'x';
  }
}

void driveMotors(int pos, int speed_) {
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
    driveMotors(p.direction, p.pace);
    delay(p.duration);
  }
  while(true) {}
}
