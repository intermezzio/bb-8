#include <Adafruit_MotorShield.h>
#include <SoftwareSerial.h>

SoftwareSerial altSerial(2,3); // RX, TX

// initialize motor shield
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// define motors
Adafruit_DCMotor *leftMotor = AFMS.getMotor(3);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(4);

String command = "";                                    // commands from the Python command line will be collected and parsed here

struct motorspeeds {
  int leftMotorSpeed;
  int rightMotorSpeed;
};

void setup() {
  Serial.begin(9600);
  AFMS.begin();
  altSerial.begin(9600);

  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);
}

void loop() {
  if (Serial.available()){
    char ch = Serial.read();                            // read the first available character

    if (ch == '\r'){                                    // if it is the end of the message
      parse_command();                                  // call parse_command
      command = "";                                     // reset the command string
    }
    else{
      command += ch;                                    // otherwise, append the character to the command string until the command is complete
    }
  }  
}

void get_input() {
  if (Serial.available()){
    char ch = Serial.read();                            // read the first available character

    if (ch == '\r'){                                    // if it is the end of the message
      parse_command();                                  // call parse_command
      command = "";                                     // reset the command string
    }
    else{
      command += ch;                                    // otherwise, append the character to the command string until the command is complete
    }
  }
}

void parse_command() {
  uint16_t val;                                         // general variable to store a value that is sent through the serial connection

  if (command.equals("w")) {
    motorspeeds forward_command = {50, 50};
    driveMotors(forward_command);
    Serial.print(forward_command.leftMotorSpeed);
    Serial.print(",");
    Serial.print(forward_command.rightMotorSpeed);
    Serial.print("\r\n");
  } 
  else if (command.equals("a")) {
    motorspeeds left_command = {30, 50};
    driveMotors(left_command);
    Serial.print(left_command.leftMotorSpeed, left_command.rightMotorSpeed);
    Serial.print("\r\n");
  }
  else if (command.equals("s")) {
    motorspeeds back_command = {-50, -50};
    driveMotors(back_command);
    Serial.print(back_command.leftMotorSpeed, back_command.rightMotorSpeed);
    Serial.print("\r\n");
  }
  else if (command.equals("d")) {
    motorspeeds right_command = {50, 30};
    driveMotors(right_command);
    Serial.print(right_command.leftMotorSpeed, right_command.rightMotorSpeed);
    Serial.print("\r\n");
  }
  else if (command.equals("esc")) {
    leftMotor->run(RELEASE);
    rightMotor->run(RELEASE);
  }
}

void driveMotors(motorspeeds speeds) {
  int leftMotorSpeed = speeds.leftMotorSpeed;
  int rightMotorSpeed = speeds.rightMotorSpeed;
  int leftMotorDirection = FORWARD;
  int rightMotorDirection = FORWARD;
  
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
