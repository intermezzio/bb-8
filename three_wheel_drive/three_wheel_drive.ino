#include <Adafruit_MotorShield.h>
#include <SoftwareSerial.h>

SoftwareSerial altSerial(2,3); // RX, TX

// initialize motor shield
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// define motors
Adafruit_DCMotor *frontMotor = AFMS.getMotor(2);
Adafruit_DCMotor *leftMotor = AFMS.getMotor(3);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(4);

String command = "";                                    // commands from the Python command line will be collected and parsed here

struct motorspeeds {
  int frontMotorSpeed;
  int leftMotorSpeed;
  int rightMotorSpeed;
};

void setup() {
  Serial.begin(9600);
  AFMS.begin();
  altSerial.begin(9600);

  frontMotor->run(FORWARD);
  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);
}

void loop() {
  get_input();
}

void get_input() {
  if (altSerial.available()){
    char ch = altSerial.read();                            // read the first available character
    
    if (ch == '\n'){                                    // if it is the end of the message
      parse_command();                                  // call parse_command
      command = "";                                     // reset the command string
    }
    else {
      command += ch;                                    // otherwise, append the character to the command string until the command is complete
    }
  }
}

// Edit this function depending on how info is sent to the arduino
void parse_command() {
    
}

void driveMotors(motorspeeds speeds) {
  int frontMotorSpeed = speeds.frontMotorSpeed;
  int leftMotorSpeed = speeds.leftMotorSpeed;
  int rightMotorSpeed = speeds.rightMotorSpeed;
  int frontMotorDirection = FORWARD;
  int leftMotorDirection = FORWARD;
  int rightMotorDirection = FORWARD;

  if (frontMotorSpeed < 0) {
    frontMotorSpeed = -frontMotorSpeed;
    frontMotorDirection = BACKWARD;
  }
  if(leftMotorSpeed < 0) {
    leftMotorSpeed = -leftMotorSpeed;
    leftMotorDirection = BACKWARD;
  }
  if(rightMotorSpeed < 0) {
    rightMotorSpeed = -rightMotorSpeed;
    rightMotorDirection = BACKWARD;
  }

  frontMotor->run(frontMotorDirection);
  leftMotor->run(leftMotorDirection);
  rightMotor->run(rightMotorDirection);

  frontMotor->setSpeed(frontMotorSpeed);
  leftMotor->setSpeed(leftMotorSpeed);
  rightMotor->setSpeed(rightMotorSpeed);
  
  return;
}
