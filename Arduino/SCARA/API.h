void chooseAction(String str) {
  if (str.length() > 0) {
    //Serial.println(str);
    String subStr;
    int ind1 = str.indexOf(';');
    subStr = str.substring(0, ind1);
    //Serial.print("subStr = ");
    //Serial.println(subStr);
    
    subStr.trim();
    //Serial.print("subStr.trim() = ");
    //Serial.println(subStr);

    //Absolute
    if(subStr.equalsIgnoreCase("FK")){
      int q1, q2, q3, q4;
      Serial.println("Forward Kinematics");
      String subStr2;
      int ind2 = str.indexOf(';', ind1 + 1);
      subStr = str.substring(ind1 + 1, ind2);
      
      /*
      StringToInt(subStr, &q1, &q2, &q3, &q4);
      Serial.print("q1 = "); Serial.print(q1);
      Serial.print(", q2 = "); Serial.print(q2);
      Serial.print(", q3 = "); Serial.print(q3);
      Serial.print(", q4 = "); Serial.println(q4);
      */

      StringToJointPos(subStr, &JP);
      if(!isInJointRange(JP)) {
        JP = JP_z1;
      }
      P = computeFK(JP);

      // Move Robot
      if(JP_z1.q4 - JP.q4 < 0){
        //Moving Down
        moveLinearTo(JP_z1, JP, true);
      }
      else {
        moveLinearTo(JP_z1, JP, false);
      }

      // Update Previous Values:
      if(isInWorkspace(P)) P_z1 = P;
      if(isInJointRange(JP)) JP_z1 = JP;
    }
    
    else if(subStr.equalsIgnoreCase("IK")){
      int x, y, z, theta;
      Serial.println("Inverse Kinematics");
      String subStr2;
      int ind2 = str.indexOf(';', ind1 + 1);
      subStr = str.substring(ind1 + 1, ind2);

      /*
      StringToInt(subStr, &x, &y, &z, &theta);
      Serial.print("x = "); Serial.print(x);
      Serial.print(", y = "); Serial.print(y);
      Serial.print(", z = "); Serial.print(z);
      Serial.print(", theta = "); Serial.println(theta);
      */

      StringToPosition(subStr, &P);
      if(!Position_isEqual(P, P_z1)) {
        printPosition(P); endl;
        if(!isInWorkspace(P)) {
          // Set the actual position to the previous position (we know it's valid)
          Serial.println("Is in Workspace ? No");
          P = P_z1;
        }
      }

      // Compute Inverse Kinematics
      if(isInWorkspace(P) && !Position_isEqual(P, P_z1)) {
        JP = computeIK(P, P_z1);
        printIK(P); endl;
        printFK(JP); endl;
        endl;
      }

      // Move Robot
      moveLinearTo(JP_z1, JP);
      /*if(JP_z1.q4 - JP.q4 < 0){
        //Moving Down
        moveLinearTo(JP_z1, JP, false);
      }
      else {
        moveLinearTo(JP_z1, JP, true);
      }*/

      // Update Previous Values:
      if(isInWorkspace(P)) P_z1 = P;
      if(isInJointRange(JP)) JP_z1 = JP;
    }

    // Relative
    if(subStr.equalsIgnoreCase("+FK")){
      int q1, q2, q3, q4;
      Serial.println("Realtive Forward Kinematics");
      String subStr2;
      int ind2 = str.indexOf(';', ind1 + 1);
      subStr = str.substring(ind1 + 1, ind2);
      
      /*
      StringToInt(subStr, &q1, &q2, &q3, &q4);
      Serial.print("q1 = "); Serial.print(q1);
      Serial.print(", q2 = "); Serial.print(q2);
      Serial.print(", q3 = "); Serial.print(q3);
      Serial.print(", q4 = "); Serial.println(q4);
      */
      JointPos relativeJP;
      JointPos checkJP = JP;
      StringToJointPos(subStr, &relativeJP);
      checkJP.q1 += relativeJP.q1;
      checkJP.q2 += relativeJP.q2;
      checkJP.q3 += relativeJP.q3;
      checkJP.q4 += relativeJP.q4;
      JP = checkJP;
      if(!isInJointRange(JP)) {
        JP = JP_z1;
      }
      P = computeFK(JP);

      // Move Robot
      SERVO1_WRITE(JP.q1);
      SERVO2_WRITE(JP.q2+90);
      SERVO3_WRITE(JP.q3+90); 
      gotoZ(relativeJP.q4);

      // Update Previous Values:
      if(isInWorkspace(P)) P_z1 = P;
      if(isInJointRange(JP)) JP_z1 = JP;
    }
    
    else if(subStr.equalsIgnoreCase("Place")){
      Serial.println("Place");
      place();
    }
    
    else if(subStr.equalsIgnoreCase("Pick")){
      Serial.println("Pick");
      pick();
    }
    
    else if(subStr.equalsIgnoreCase("Delay")){
      int T, x;
      String subStr2;
      int ind2 = str.indexOf(';', ind1 + 1);
      subStr = str.substring(ind1 + 1, ind2);
      StringToInt(subStr, &T, &x, &x, &x);
      Serial.print("Delay("); Serial.print(T); Serial.println(")");
      _delay_ms_(T);
    }
    
    else{
      /*Serial.print("Command \"");
      Serial.print(subStr);
      Serial.println("\" was not recognized");*/
    }
  }
}


void commander(String str) {
  if(str.length() > 0){
    int i, count;
    for (i=0, count=0; str[i]; i++){
      count += (str[i] == '\n');
      //if(str[i] == '\n') Serial.println(i);
    }
    //Serial.print("Count('\\n') = ");
    //Serial.println(count);
    
    String strr_ = str;
    String subStrr;
    String subStrrr;
    for (int j=0; j<count; j++){
      int in1 = strr_.indexOf('\n');
      subStrr = strr_.substring(0, in1);
      strr_ = strr_.substring(in1+1, strr_.length()-1);
      subStrrr = subStrr;
      chooseAction(subStrrr);
      //Serial.print("str_ = \""); Serial.print(strr_); Serial.println("\"");
      //Serial.print("subStrr = \""); Serial.print(subStrr); Serial.println("\"");
      //Serial.println("");
    }
  }
}
