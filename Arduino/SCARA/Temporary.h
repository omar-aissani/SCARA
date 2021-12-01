void z_go(int rot, bool dir){
  digitalWrite(DIR_PIN, dir);
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < rot*200; x++) {
    digitalWrite(STEP_PIN, HIGH); 
    delayMicroseconds(500); 
    digitalWrite(STEP_PIN, LOW); 
    delayMicroseconds(500); 
  }
}

void goTo(JointPos jp, unsigned long sleepTime = 0) {
  if(sleepTime == 0) {
    SERVO1_WRITE(jp.q1);
    SERVO2_WRITE(jp.q2+90);
    SERVO3_WRITE(jp.q3+90);  
  }
  else{
    SERVO1_WRITE(jp.q1);
    _delay_ms(sleepTime);
    SERVO2_WRITE(jp.q2+90);
    _delay_ms(sleepTime);
    SERVO3_WRITE(jp.q3+90);
    _delay_ms(sleepTime);  
  }
}

void linearTo(JointPos From, JointPos To, int N = 150) {
  SERVO1_WRITE(From.q1);
  SERVO2_WRITE(From.q2+90);
  SERVO3_WRITE(From.q3+90);

  JointVel JV;

  JV.dq1 = (double)(To.q1 - From.q1)/N;
  JV.dq2 = (double)(To.q2 - From.q2)/N;
  JV.dq3 = (double)(To.q3 - From.q3)/N;
  
  for(int i=0; i<N; i++){
    SERVO1_WRITE(From.q1 + i*JV.dq1);
    SERVO2_WRITE(From.q2+90 + i*JV.dq2);   
    SERVO3_WRITE(From.q3+90 + i*JV.dq3);
    delay(5);
  }
}

// Taskspace
void linearTo(Position From, Position To, int N = 150) {
  JointPos JP_From = computeIK(From);
  JointPos JP_To = computeIK(To);
  
  SERVO1_WRITE(JP_From.q1);
  SERVO2_WRITE(JP_From.q2+90);
  SERVO3_WRITE(JP_From.q3+90);

  JointVel JV;


  JV.dq1 = (double)(JP_To.q1 - JP_From.q1)/N;
  JV.dq2 = (double)(JP_To.q2 - JP_From.q2)/N;
  JV.dq3 = (double)(JP_To.q3 - JP_From.q3)/N;
  
  for(int i=0; i<N; i++){
    SERVO1_WRITE(JP_From.q1 + i*JV.dq1);
    SERVO2_WRITE(JP_From.q2+90 + i*JV.dq2);   
    SERVO3_WRITE(JP_From.q3+90 + i*JV.dq3);
    delay(5);
  }
}


// Z 
void homeZ() {
  unsigned long dt = 500;
  digitalWrite(DIR_PIN, DOWN);
  Serial.println("Homing Z");
  while(1) {
    digitalWrite(STEP_PIN, HIGH); 
    delayMicroseconds(dt); 
    digitalWrite(STEP_PIN, LOW); 
    delayMicroseconds(dt);

    if(digitalRead(ENDSTOP_PIN) == LOW){
      Serial.println("Stop Z");
      break;
    } 
  }
}

void gotoZ(int z) {
  // 1tr -> pitch = 4mm
  // 1tr -> step/rev = 200step
  // Xmm -> X*25step;
  digitalWrite(EN_PIN, LOW); // Enable a4988
  bool dir = z<0 ? DOWN:UP;
  digitalWrite(DIR_PIN, dir);
  unsigned long dt = 500;
  unsigned int absZ = abs(z);
  
  for(unsigned int x = 0; x < absZ*25; x++) {
    digitalWrite(STEP_PIN, HIGH); 
    delayMicroseconds(dt); 
    digitalWrite(STEP_PIN, LOW); 
    delayMicroseconds(dt);
    
    if(digitalRead(ENDSTOP_PIN) == LOW && dir == DOWN){
      Serial.println("Stop");
      break;
    }
  }
  digitalWrite(EN_PIN, HIGH); // Disable a4988
}

// End Effector
void pick() {
  digitalWrite(SOLENOID_PIN, LOW);
}

void place() {
  digitalWrite(SOLENOID_PIN, HIGH);
}


void pickFrom(Position Pos) {
  
}

void placeAt(Position Pos) {
  
}

void moveLinearTo(JointPos From, JointPos To, boolean isRot_Z = true, int N = 150) {
  if(To.q4 - From.q4 < 0){
    //Moving Down
    linearTo(From, To);
    gotoZ(To.q4 - From.q4);
  }
  else {
    gotoZ(To.q4 - From.q4);
    linearTo(From, To);
  }
}
