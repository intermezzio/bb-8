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

unsigned long timeOfLastCommand;

struct motorspeeds {
  int frontMotorSpeed;
  int leftMotorSpeed;
  int rightMotorSpeed;
};

void setup() {
  timeOfLastCommand = millis();
  
  Serial.begin(9600);
  AFMS.begin();
  altSerial.begin(9600);

  frontMotor->run(FORWARD);
  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);
}

void loop() {
  get_input();
  if( (millis() - timeOfLastCommand) > 20000 ) {        // if it's been 20 seconds since a command was given
    motorspeeds stop_command = {0, 0, 0};
    driveMotors(stop_command);
    Serial.println("Stopped due to inactivity");
    timeOfLastCommand = millis();
  }
}

void get_input() {
  if (altSerial.available()){
    char ch = altSerial.read();                            // read the first available character
    
    if (ch == '\r'){                                    // if it is the end of the message
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
    int max_motor_speed = 255;
    int first_comma_index = command.indexOf(',');
    int second_comma_index = command.indexOf(',', first_comma_index + 1);
    float front_motor_speed = command.substring(0, first_comma_index).toFloat() * max_motor_speed;
    float left_motor_speed = command.substring(first_comma_index + 1, second_comma_index).toFloat() * max_motor_speed;
    float right_motor_speed = command.substring(second_comma_index + 1).toFloat() * max_motor_speed;

    motorspeeds update_command = {front_motor_speed, left_motor_speed, right_motor_speed};

    driveMotors(update_command);
    
    altSerial.println("Sent");

    timeOfLastCommand = millis();
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
