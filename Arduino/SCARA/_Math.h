#define DEG2RAD  0.0174533
#define RAD2DEG 57.2957795

// Matlab-like Functions
double linspace(double x, double X, unsigned int n, unsigned int i){
  double step = (X-x)/n;
  return x + (i)*step; 
}

double range(double x, double step, double X, unsigned int i){
  return x + (i)*step; 
}

// Trigonometry 
double cosd(double psi) {
  double PSI = psi * DEG2RAD;
  return cos(PSI);
}
double sind(double psi) {
  double PSI = psi * DEG2RAD;
  return sin(PSI);
}
double tand(double psi) {
  double PSI = psi * DEG2RAD;
  return tan(PSI);
}
double acosd(double x) {
  return acos(x) * RAD2DEG;
}
double asind(double x) {
  return asin(x) * RAD2DEG;
}
double atand(double x) {
  return atan(x) * RAD2DEG;
}
double atan2d(double y, double x) {
  return atan2(y, x) * RAD2DEG;
}
double wrapTo180(double x) {
    x = fmod(x+180, 360);
    if (x < 0)
        x += 360;
    return x - 180;
}

// Linear
double affine(double x, double a, double b){
  return a*x + b;
}
