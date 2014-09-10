#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

/* ========================================================
    DEFINING ALL VARIABLES
* ========================================================= */

MPU6050 mpu; // MPU 6050 object

bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer
Quaternion q;           // [eta, e_1, e_2, e_3]         quaternion container
float euler[3];         // [psi, theta, phi]    Euler angle container
int16_t ax, ay, az, gx, gy, gz;
float pitch, roll, yaw;

// Interrupt detection routine
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

// used to get size of time-step
unsigned long start_time;
unsigned long end_time;
unsigned long delta;

void setup () {
  
  Serial.begin(9600);
  while(!Serial); // Wait for Leonardo enumeration
  delay(5000);    // Gives time to open Serial Monitor
    
  setup_mpu();
  
  
  char to_from_PC = 'a';
  int temp;
  while (to_from_PC != 's'){ // Wait for connection with matlab
    Serial.println(to_from_PC);
    delay(500);
    temp = Serial.read();
    if (temp != -1) {
      to_from_PC = temp;
    } 
  }
  Serial.println(to_from_PC); // Sends confimation to matlab that data was recieved
  delay(500);
  
  start_time = micros();
}


void loop() {

  // if programming failed, dont do anything
  if (!dmpReady) return;
  
  while (!mpuInterrupt && fifoCount < packetSize) { // wait for MPU interrupt or if extra packet(s) available
    // do control?
  }
  
  get_attitude();  
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  pitch = atan2(ax/sqrt(ay^2+az^2));
  roll = atan2(ay/sqrt(ax^2+az^2));
  
//  delta = micros()-end_time;
  end_time = micros()-start_time;
  
  if (euler[0] == 0 && euler[0] == 0 && euler[0] == 0){
    // no data received yet
  }else {
    
    // Times 1000 at the end to scale up data. Gets more information through to matlab
    Serial.println(end_time);
    Serial.println(euler[0] * 180/M_PI * 1000);
    Serial.println(euler[1] * 180/M_PI * 1000);
    Serial.println(euler[2] * 180/M_PI * 1000);
    Serial.println(pitch * 180/M_PI * 1000);
    Serial.println(roll * 180/M_PI * 1000);
    Serial.println(gx * 180/M_PI * 1000);
    Serial.println(gy * 180/M_PI * 1000);
    Serial.println(gz * 180/M_PI * 1000);
  }
}
