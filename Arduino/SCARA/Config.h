// Assumption:
// ANGLE (Upper Case) --> Rad
// angle (Lower Case) --> Degrees
// Default units are Degrees, mm, g

// Note:
// We are working with integers to make life easier
// All float values are rounded up

// Parameters
unsigned long L1 = 150;
unsigned long L2 = 150;
unsigned long L3 = 61;

// Joint Limits m = min, M = Max
int q1m = -20, q1M = 210;
int q2m = -90-40, q2M = 90+40;
int q3m = -90-40, q3M = 90+40;
int q4m = 0, q4M = 300;



// Home Position
// Revolute Joints
uint16_t q1_HOME = 90;
uint16_t q2_HOME = 0;
uint16_t q3_HOME = 0;
// Prismatic Joint
uint16_t q4_HOME = 150;


// End Stop Pin
#define ENDSTOP_PIN 2
// Pump Pin
#define PUMP_PIN 8
#define SOLENOID_PIN 8
// Servos Pins
#define SERVO1_PIN 3
#define SERVO2_PIN 5
#define SERVO3_PIN 6
// Stepper Motor Pins
#define STEP_PIN 12
#define DIR_PIN 13
#define EN_PIN 11


// For Z_go function
#define UP 1
#define DOWN 0


// Servo Calibration
const double SERVO1_A = 7.1;
const double SERVO1_B = 864;
const double SERVO2_A = 7.139;
const double SERVO2_B = 850;
const double SERVO3_A = 7.055;
const double SERVO3_B = 850;

// Servo Write (Calibrated)
//#define SERVO1_WRITE(x) servo1.write(SERVO1_A * (x) + SERVO1_B)
//#define SERVO2_WRITE(x) servo2.write(SERVO2_A * (x) + SERVO2_B)
//#define SERVO3_WRITE(x) servo3.write(SERVO3_A * (x) + SERVO3_B)

void SERVO1_WRITE(int x){
  unsigned int a;
  a = SERVO1_A * x + SERVO1_B;
  servo1.write(a);
}

void SERVO2_WRITE(int x){
  unsigned int a;
  a = SERVO2_A * x + SERVO2_B;
  servo2.write(a);
}

void SERVO3_WRITE(int x){
  unsigned int a;
  a = SERVO3_A * x + SERVO3_B;
  servo3.write(a);
}
