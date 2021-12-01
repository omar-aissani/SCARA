//-----------------------------------------//
//           FORWARD KINEMATICS            //
//-----------------------------------------//

// Short for JointPosition
typedef struct JointPos JointPos;
struct JointPos {
  int q1; // Theta1
  int q2; // Theta2
  int q3; // Theta3
  int q4; // Z
};

// Short for JointVelocity
typedef struct JointVel JointVel;
struct JointVel {
  double dq1; // dTheta1_dt
  double dq2; // dTheta2_dt
  double dq3; // dTheta3_dt
  double dq4; // dZ_dt
};

// Short for JointAcceleration
typedef struct JointAccel JointAccel;
struct JointAccel {
  double ddq1; // d²Theta1_d²t
  double ddq2; // d²Theta2_d²t
  double ddq3; // d²Theta3_d²t
  double ddq4; // d²Z_d²t
};


//-----------------------------------------//
//           INVERSE KINEMATICS            //
//-----------------------------------------//

typedef struct Position Position;
struct Position {
  int x;
  int y;
  int z;
  int phi;
};

typedef struct Velocity Velocity;
struct Velocity {
  double dx; // dx_dt
  double dy; // dy_dt
  double dz; // dz_dt
  double dphi; // dphi_dt
};

typedef struct Acceleration Acceleration;
struct Acceleration {
  double ddx; // d²x_dt²
  double ddy; // d²x_dt²
  double ddz; // d²z_dt²
  double ddphi; // d²phit_dt²
};



// Forward Kinematics
Position computeFK(JointPos JP) {
  Position Pos;
  Pos.x = round(L1*cosd(JP.q1) + L2*cosd(JP.q1+JP.q2) + L3*cosd(JP.q1+JP.q2+JP.q3));
  Pos.y = round(L1*sind(JP.q1) + L2*sind(JP.q1+JP.q2) + L3*sind(JP.q1+JP.q2+JP.q3));
  Pos.z = round(JP.q4);
  Pos.phi = round(JP.q1 + JP.q2 + JP.q3);
  return Pos;
}

// Inverse Kinematics 2 Link Planar Robot
JointPos computeIK_2DOF(Position Pos, boolean isElbowDown=true){
  double q1, q2, q3, q4;
  JointPos JP = {q1m-10, q2m-10, q3m-10, q4m-10};
  
  double x_ = Pos.x;
  double y_ = Pos.y;
  unsigned long A2 = L2+L3;
  double a = (x_*x_ + y_*y_ - L1*L1 - A2*A2) / (2* L1*A2);
  
  if(a > 1 || a < -1) {
    Serial.println("No solution");
  }
  else {
    if(isElbowDown) {
      JP.q2 = acosd(a);
      JP.q1 = wrapTo180(atan2d(y_, x_) - atan2d(A2*sind(JP.q2), L1 + A2*cosd(JP.q2))); // Elbow Down
      JP.q3 = 0;
    }
    else {
      JP.q2 = -acosd(a);
      JP.q1 = wrapTo180(atan2d(y_, x_) - atan2d(A2*sind(JP.q2), L1 + A2*cosd(JP.q2))); // Elbow Up
      JP.q3 = 0;  
    }
    JP.q4 = Pos.z;
  }
  return JP;
}

// Inverse Kinematics
JointPos computeIK(Position Pos, boolean isElbowDown=true){
  double q1, q2, q3, q4;
  JointPos JP = {q1m-10, q2m-10, q3m-10, q4m-10};
  
  //Pos.phi = (360-Pos.phi);
  double x_ = Pos.x - L3 * cosd(Pos.phi);
  double y_ = Pos.y - L3 * sind(Pos.phi);
  double a = (x_*x_ + y_*y_ - L1*L1 - L2*L2) / (2* L1*L2);
  
  if(a > 1 || a < -1) {
    Serial.println("No solution");
  }
  else {
    if(isElbowDown) {
      JP.q2 = acosd(a);
      JP.q1 = wrapTo180(atan2d(y_, x_) - atan2d(L2*sind(JP.q2), L1 + L2*cosd(JP.q2))); // Elbow Down
      JP.q3 = wrapTo180(Pos.phi - JP.q1 - JP.q2);
    }
    else {
      JP.q2 = -acosd(a);
      JP.q1 = wrapTo180(atan2d(y_, x_) - atan2d(L2*sind(JP.q2), L1 + L2*cosd(JP.q2))); // Elbow Up
      JP.q3 = wrapTo180(Pos.phi - JP.q1 - JP.q2);  
    }
    JP.q4 = Pos.z;
  }
  return JP;
}

// return whether or not the position is reachable
bool isInWorkspace(Position Pos) {
  boolean isIn = true;
  //JointPos JP = computeIK(Pos, true);
  JointPos JP = computeIK_2DOF(Pos, true);
  if(JP.q1 < q1m || JP.q2 < q2m || JP.q3 < q3m || JP.q4 < q4m) {
    isIn = false;
    //return false;
  }
  if(JP.q1 > q1M || JP.q2 > q2M || JP.q3 > q3M || JP.q4 > q4M) {
    isIn = false;
    //return false;
  }

  if(isIn == true){
    return true;
  }
  
  isIn = true;
  //JP = computeIK(Pos, false);
  JP = computeIK_2DOF(Pos, false);
  if(JP.q1 < q1m || JP.q2 < q2m || JP.q3 < q3m || JP.q4 < q4m) {
    isIn = false;
    //return false;
  }
  if(JP.q1 > q1M || JP.q2 > q2M || JP.q3 > q3M || JP.q4 > q4M) {
    isIn = false;
    //return false;
  }
  return isIn;
}

// return whether or not the joint surpass the joint limits
bool isInJointRange(JointPos JP){
  if(JP.q1 < q1m || JP.q2 < q2m || JP.q3 < q3m || JP.q4 < q4m) {
    return false;
  }
  if(JP.q1 > q1M || JP.q2 > q2M || JP.q3 > q3M || JP.q4 > q4M) {
    return false;
  }
  return true;
}

// Inverse Kinematics Safe
JointPos computeIK(Position Pos, Position Pos_z1){
  JointPos JP;
  //JP = computeIK(Pos);
  JP = computeIK_2DOF(Pos, true);
  if(!isInJointRange(JP)){
    Serial.println("Elbow Down, In Joint Range ? No");
    JP = computeIK_2DOF(Pos, false);
    if(!isInJointRange(JP)){
      Serial.println("Elbow Up, In Joint Range ? No");
      //return computeIK(Pos_z1);
      return computeIK_2DOF(Pos_z1); 
    }
  }
  Serial.println("In Joint Range ? Yes");
  return JP;
}

void printJointPos(JointPos JP) {
  Serial.print("q1: ");
  Serial.print(JP.q1);
  Serial.print(", q2: ");
  Serial.print(JP.q2);
  Serial.print(", q3: ");
  Serial.print(JP.q3);
  Serial.print(", q4: ");
  Serial.print(JP.q4);
}

void printPosition(Position Pos){
  Serial.print("x: ");
  Serial.print(Pos.x);
  Serial.print(", y: ");
  Serial.print(Pos.y);
  Serial.print(", z: ");
  Serial.print(Pos.z);
  Serial.print(", phi: ");
  Serial.print(Pos.phi);
}

void printFK(JointPos JP){
  printJointPos(JP);
  Serial.print(" --> ");
  Position P = computeFK(JP);
  printPosition(P);
}

void printIK(Position P){
  printPosition(P);
  Serial.print(" --> ");
  JointPos JP = computeIK(P);
  printJointPos(JP);
}


// Parsing Functions
void StringToJointPos(String str, JointPos *JP) {
  if (str.length() > 0) {
    String subStr;
    int ind1 = str.indexOf(',');
    subStr = str.substring(0, ind1);
    JP->q1 = subStr.toDouble();
  
    int ind2 = str.indexOf(',', ind1 + 1);
    subStr = str.substring(ind1 + 1, ind2);
    JP->q2 = subStr.toDouble();
  
    int ind3 = str.indexOf(',', ind2 + 1);
    subStr = str.substring(ind2 + 1, ind3);
    JP->q3 = subStr.toDouble();
  
    int ind4 = str.indexOf(',', ind3 + 1);
    subStr = str.substring(ind3 + 1, ind4);
    JP->q4 = subStr.toDouble();
  }
}

void StringToPosition(String str, Position *Pos) {
  if (str.length() > 0) {
    String subStr;
    int ind1 = str.indexOf(',');
    subStr = str.substring(0, ind1);
    Pos->x = subStr.toDouble();
  
    int ind2 = str.indexOf(',', ind1 + 1);
    subStr = str.substring(ind1 + 1, ind2);
    Pos->y = subStr.toDouble();
  
    int ind3 = str.indexOf(',', ind2 + 1);
    subStr = str.substring(ind2 + 1, ind3);
    Pos->z = subStr.toDouble();
  
    int ind4 = str.indexOf(',', ind3 + 1);
    subStr = str.substring(ind3 + 1, ind4);
    Pos->phi = subStr.toDouble();
  }
}


// Comparing Functions
bool Position_isEqual(Position P1, Position P2) {
  if(P1.x == P2.x && P1.y == P2.y && P1.z == P2.z && P1.phi == P2.phi) {
    return true;
  }
  return false;
}

bool JointPos_isEqual(JointPos JP1, JointPos JP2) {
  if(JP1.q1 == JP2.q1 && JP1.q2 == JP2.q2 && JP1.q3 == JP2.q3 && JP1.q4 == JP2.q4) {
    return true;
  }
  return false;
}

// End of Code
