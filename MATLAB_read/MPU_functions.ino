
/* ============================================================
This code is a part of a project at NTNU used for retrieving the quaternions for the system
   ============================================================ */

/* =========================================================================
   Connections:
   INT to pin 1
   SDA to pin 2
   SCL to pin 3
   GND to GND
   VCC to 3.3V or 5V
 * ========================================================================= */


void setup_mpu(){ 
  
  Wire.begin();
  TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz) Not sure why this is here... Remove?  
  mpu.initialize();
  devStatus = mpu.dmpInitialize();
  mpu.setXAccelOffset(-3030);
  mpu.setYAccelOffset(1740);
  mpu.setZAccelOffset(947); 
  mpu.setXGyroOffset(13);
  mpu.setYGyroOffset(2);
  mpu.setZGyroOffset(20); 

  if (devStatus == 0) {
    mpu.setDMPEnabled(true);
    attachInterrupt(3, dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();
    
    dmpReady = true;

    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    //ERROR!
  }
}

float get_attitude(){
  
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();
  
  fifoCount = mpu.getFIFOCount();

  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    mpu.resetFIFO();

  } else if (mpuIntStatus & 0x02) {
    // wait for correct available data length, should be a VERY short wait
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

    // read a packet from FIFO
    mpu.getFIFOBytes(fifoBuffer, packetSize);
        
    // track FIFO count here in case there is > 1 packet available
    // (this lets us immediately read more without waiting for an interrupt)
    fifoCount -= packetSize;
    
    
    
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetEuler(euler, &q);
  }
}


