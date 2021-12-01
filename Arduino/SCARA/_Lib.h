// DEBUG_MODE
#define DEBUG_MODE 0
#define RELEASE_MODE 1

#include "_math.h"

#define endl Serial.println("")

// To get the elapsed time from the tic moment
unsigned long _T0_;
#define tic _T0_ = micros()
#define toc micros() - _T0_


// Custom delayMicroSeconds function non blocking
void _delay_us_(unsigned long t) {
  unsigned long t0 = micros();
  while (micros() - t0 < t) {
    // do nothing
  }
}
// Custom delay function non blocking
void _delay_ms_(unsigned long t) {
  unsigned long t0 = millis();
  while (millis() - t0 < t) {
    // do nothing
  }
}



// Read String from Serial Monitor
String readSerial() {
  String str;
  if (Serial.available()) {
    //tic;
    while (Serial.available()) {
      char c = Serial.read();
      str += c;
      _delay_us_(500);
    }
    //Serial.print(toc);
    //Serial.print("\t");
  }
  return str;
}

String readSerialVerbose() {
  String str;
  if (Serial.available()) {
    //tic;
    while (Serial.available()) {
      char c = Serial.read();
      str += c;
      _delay_us_(500);
    }
    Serial.print("Serial Data: ");
    Serial.println(str);
    //Serial.print(toc);
    //Serial.print("\t");
  }
  return str;
}


// Parse csv from Serial to integers (q1, q2, q3, q4)
void StringToInt(String str, int *q1, int *q2, int *q3, int *q4) {
  //unsigned long t0 = micros(); // Measure performance of the code
  
  if (str.length() > 0) {
    String subStr;
    
    // This function has a little problem:
    // If we enter "x, y, z" it will work fine.
    // When we enter "x" all the values will be equal to x
    // Example:
    // "30, 90, 150" ==> q1 = 30, q2 = 90, q3 = 150
    // "30" ==> q1 = 30, q2 = 30, q3 = 30
    // "30, 90" ==> q1 = 30, q2 = 90, q3 = 30

    // I should rewrite this function properly later
    // For now i'll leave it as it is since i don't have time
    // It should work for N variables
    
    int ind1 = str.indexOf(',');
    subStr = str.substring(0, ind1);
    *q1 = subStr.toInt();

    int ind2 = str.indexOf(',', ind1 + 1);
    subStr = str.substring(ind1 + 1, ind2);
    *q2 = subStr.toInt();

    int ind3 = str.indexOf(',', ind2 + 1);
    subStr = str.substring(ind2 + 1, ind3);
    *q3 = subStr.toInt();

    int ind4 = str.indexOf(',', ind3 + 1);
    subStr = str.substring(ind3 + 1, ind4);
    *q4 = subStr.toInt();

    if (0) {
      Serial.print("q1 = ");
      Serial.print(*q1);

      Serial.print("\tq2 = ");
      Serial.print(*q2);

      Serial.print("\tq3 = ");
      Serial.println(*q3);

      Serial.print("\tq4 = ");
      Serial.println(*q4);
      
      //Serial.print("time [µs] elapsed since you called the function: ");
      //Serial.println(micros() - t0);
    }

  }
}




// End of code
