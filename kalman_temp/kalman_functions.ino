

// states = (angle, bias_dot)
double Q_angle = 0.001;
double Q_bias = 0.003;
double R_measure = 0.03;
double angle = 0;
double bias = 0;
double rate;
double P[2][2] = {{0, 0},{0, 0}};
double K[2];
double y;
double S;

double getAngle(double newAngle, double newRate, double dt){
  rate = euler[0]-bias;
  angle += dt * rate;
  
  P[0][0] += dt * (dt*P[1][1] - P[0][1] -P[1][0] + Q_angle);
  P[0][1] -= dt * P[1][1];
  P[1][0] -= dt * P[1][1];
  P[1][1] -= Q_bias * dt;
  
  S = P[0][0] + R_measure;
  
  K[0] = P[0][0]/S;
  K[1] = P[1][0]/S;
  
  y = newAngle - angle;
  
  angle += K[0] * y;
  bias += K[1] * y;
  
  P[0][0] -= K[0] * P[0][0];
  P[0][1] -= K[0] * P[0][1];
  P[1][0] -= K[1] * P[0][0];
  P[1][1] -= K[1] * P[0][1];
  
  return angle;
}
