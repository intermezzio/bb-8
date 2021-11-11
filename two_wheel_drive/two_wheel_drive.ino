#include <Adafruit_MotorShield.h>
#include <SoftwareSerial.h>

SoftwareSerial altSerial(2,3); // RX, TX

// initialize motor shield
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// define motors
Adafruit_DCMotor *leftMotor = AFMS.getMotor(3);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(4);

int speed_ = 6;

struct motorspeeds {
  int leftMotorSpeed;
  int rightMotorSpeed;
};

struct path {
//  float direction;
  int duration;
  motorspeeds pace;
};

// path array
path paths[] = {
  {5000, {75, 75}},
  {5000, {-75, -75}},
  {1000, {0, 0}}
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

  altSerial.begin(9600);
  altSerial.println("Test altSerial message");
  
  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);
}

/*
motorspeeds chooseSpeeds(path p) {
  int pos = p.direction;
  int speed_ = p.pace;
  
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
*/

void getInput() {
  if(altSerial.available() == 0) {
    return;
  }
  char direction_ = altSerial.read();
  switch(direction_) {
    case 'w':
      driveMotors({5,5});
      break;
    case 'a':
      driveMotors({3,5});
      break;
    case 's':
      driveMotors({-5,-5});
      break;
    case 'd':
      driveMotors({5,3});
      break;
    default:
      return;
      break;
  }
  return;
}

String altDelay(int ms) {
  return "";
}

void driveMotors(motorspeeds speeds) {
  int leftMotorSpeed = speeds.leftMotorSpeed;
  int rightMotorSpeed = speeds.rightMotorSpeed;
  int leftMotorDirection = FORWARD;
  int rightMotorDirection = FORWARD;
  
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



void loop() {
  // for element in list of paths
  for(int i = 0; i < num_paths; i++) {
    path p = paths[i];
  // motorspeeds s = chooseSpeeds(p);
    driveMotors(p.pace);
    delay(p.duration);
  }
  leftMotor->run(RELEASE);
  rightMotor->run(RELEASE);
  while(true) {}
}
