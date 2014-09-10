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

// Interrupt detection routine
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;

void setup () {
  
  Serial.begin(115200);
  while(!Serial); // Wait for Leonardo enumeration
  delay(5000);    // Gives time to open Serial Monitor
  
  setup_mpu();
  
  // configure LED for output
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // if programming failed, dont do anything
  if (!dmpReady) return;
  
  while (!mpuInterrupt && fifoCount < packetSize) { // wait for MPU interrupt or if extra packet(s) available
    // do control?
  }
  get_attitude();
  Serial.print("quat\t");
  Serial.print(q.w);
  Serial.print("\t");
  Serial.print(q.x);
  Serial.print("\t");
  Serial.print(q.y);
  Serial.print("\t");
  Serial.println(q.z);
  
  
  // blink LED to indicate activity
  blinkState = !blinkState;
  digitalWrite(LED_PIN, blinkState);
}
