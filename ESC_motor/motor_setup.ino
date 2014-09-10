
void motor_setup(Servo firstESC){
  int initialize = 0;
  int set_zero_throttle = 1000; //Full throttle is 2000
  
  for (int i=0; i<10; i++){
    firstESC.writeMicroseconds(initialize);
    delay(500); 
  } 
  firstESC.writeMicroseconds(set_zero_throttle);
  delay(3000);
}
