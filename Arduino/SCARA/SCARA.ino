#include <Servo.h>

Servo servo1, servo2, servo3;

#include "_Lib.h"
#include "Config.h"
#include "SCARA.h"
#include "Temporary.h"


unsigned int i=0;
String readString;


Position P_z1 = {0, 361, 150, 90};
Position P = {200, 250, 150, 90};

Position P_ = {0, 360, 150, 90};

JointPos JP, JP_z1;
JointPos Home = {90, 0, 0, 150};

#include "API.h"

// Setup
void setup() {
  pinMode(ENDSTOP_PIN, INPUT_PULLUP);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(SOLENOID_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);
  //digitalWrite(EN_PIN, HIGH);
  digitalWrite(STEP_PIN, LOW);
  //digitalWrite()
  
  Serial.begin(115200);
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  

  
  delay(1000);
  Serial.println("--------------------------------------------");
  Serial.println("            Inverse Kinematics              ");
  Serial.println("--------------------------------------------");
  //delay(1000);

  homeZ();
  gotoZ(2);
  delay(1000);
  gotoZ(20);
  
  goTo(Home, 1500);
  //_delay_ms(2000);

  JP = computeIK(P); 
  JP_z1 = computeIK(P_z1);
  goTo(JP_z1);
  _delay_ms(1000);
}

// Read Position, IK, Print Joint Variables, Move Robot Acordingly
void loop() {
  readString = readSerialVerbose();
  //commander(readString);
  chooseAction(readString);
  //_delay_ms_(1000);
}
