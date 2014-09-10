
/* ============================================================
This code is a part of a project at NTNU used for retrieving the quaternions for the system
   ============================================================ */

/* =========================================================================
   NOTE: In addition to connection 3.3v or 5V, GND, SDA, and SCL, this code
   depends on the MPU-6050's INT pin being connected to the Arduino's
   external interrupt pin. On the Arduino Uno and Mega 2560, this is
   digital I/O pin 2 (edit code to use external interrupt #0). 
   On Arduino Leonardo use external interrupt pin 1
   (edit code to use external interrupt #0), 
   set SDA on pin 2 and SCL on pin 3.
 * ========================================================================= */


void setup_mpu(){ 
  
  Wire.begin();
  TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz) Not sure why this is here... Remove?
  
  Serial.println(F("Initializing I2C devices...")); // F functions stores the string in flash instead of ram
  mpu.initialize();
  
  Serial.println(F("Testing device connections..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

  // load and configure the DMP
  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();

  // To find the correct offset, run the 'raw_data' file and experiment
  mpu.setXAccelOffset(-3030);
  mpu.setYAccelOffset(1740);
  mpu.setZAccelOffset(947); 
  mpu.setXGyroOffset(13);
  mpu.setYGyroOffset(2);
  mpu.setZGyroOffset(20); 
  
  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
    // turn on the DMP, now that it's ready
//    Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);
    
    // enable Arduino interrupt detection
//    Serial.println(F("Enabling interrupt detection (Arduino external interrupt 3(pin 1))..."));
    attachInterrupt(3, dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();
    
    // set our DMP Ready flag so the main loop() function knows it's okay to use it
//    Serial.println(F("DMP ready! Waiting for first interrupt..."));
    dmpReady = true;

    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    //ERROR!
    //1 = initial memory load failed
    //2 = DMP configuration updates failed
//    Serial.print(F("DMP Initialization failed (code "));
//    Serial.print(devStatus);
//    Serial.println(F(")"));
  }
}

void get_attitude(){
  
  // reset interrupt flag and get INT_STATUS byte
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();
  
  // get current FIFO count
  fifoCount = mpu.getFIFOCount();

  // check for overflow (this should never happen unless our code is too inefficient)
  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    // reset so we can continue cleanly
    mpu.resetFIFO();
//    Serial.println(F("FIFO overflow!"));

  // otherwise, check for DMP data ready interrupt (this should happen frequently)
  } else if (mpuIntStatus & 0x02) {
    // wait for correct available data length, should be a VERY short wait
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

    // read a packet from FIFO
    mpu.getFIFOBytes(fifoBuffer, packetSize);
        
    // track FIFO count here in case there is > 1 packet available
    // (this lets us immediately read more without waiting for an interrupt)
    fifoCount -= packetSize;
    
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetEuler(euler, &q);
  }
}


