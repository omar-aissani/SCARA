// Inverse Kinematics 2 Link Planar Robot Elbow Down Pose
JointPos computeIK_2DOF_ElbowDown(Position Pos) {
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
    JP.q2 = acosd(a);
    JP.q1 = wrapTo180(atan2d(y_, x_) - atan2d(A2*sind(JP.q2), L1 + A2*cosd(JP.q2)));
    JP.q3 = 0;
    JP.q4 = Pos.z;
  }
  return JP;
}

// Inverse Kinematics 2 Link Planar Robot Elbow Up Pose
JointPos computeIK_2DOF_ElbowUp(Position Pos) {
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
    JP.q2 = -acosd(a);
    JP.q1 = wrapTo180(atan2d(y_, x_) - atan2d(A2 * sind(JP.q2), L1 + A2 * cosd(JP.q2))); // Elbow Up
    JP.q3 = 0;
    JP.q4 = Pos.z;
  }
  return JP;
}

// Inverse Kinematics 2 Link Planar Robot, Pose doesn't matter
JointPos computeIK_2DOF(Position Pos, boolean isElbowDown=true) {
  double q1, q2, q3, q4;
  JointPos JP = {q1m-10, q2m-10, q3m-10, q4m-10};
  
  double x_ = Pos.x;
  double y_ = Pos.y;
  unsigned long A2 = L2+L3;
  double a = (x_*x_ + y_*y_ - L1*L1 - A2*A2) / (2*L1*A2);
  
  if(a > 1 || a < -1) {
    Serial.println("No solution");
  }
  else {
    if(isElbowDown) {
    	JP = computeIK_2DOF_ElbowDown(Pos);
			if(!isInJointRange(JP)){
				JP = computeIK_2DOF_ElbowUp(Pos);
				if(!isInJointRange(JP)){
					JP = {q1m-10, q2m-10, q3m-10, q4m-10};
				}
			}
    }
		else {
			JP = computeIK_2DOF_ElbowUp(Pos);
			if(!isInJointRange(JP)){
				JP = computeIK_2DOF_ElbowDown(Pos);
			}
		}
    
  }
  return JP;
}


// Inverse Kinematics 2 Link Planar Robot
JointPos computeIK_2DOF(Position Pos){
  double q1, q2, q3, q4;
  JointPos JP = {q1m-10, q2m-10, q3m-10, q4m-10};
  
  double x_ = Pos.x;
  double y_ = Pos.y;
  unsigned long A2 = L2+L3;
  double a = (x_*x_ + y_*y_ - L1*L1 - A2*A2) / (2*L1*A2);
  
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