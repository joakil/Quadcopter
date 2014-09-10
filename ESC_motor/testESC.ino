#include <Servo.h>

int speed_1 = 0;
int old_speed_1 = 0;
Servo firstESC;

void setup(){
  firstESC.attach(9);
  Serial.begin(9600); 
  delay(5000);
  Serial.println("Initializing");
  motor_setup(firstESC);
  Serial.println("Started!");
}

void loop(){
  
  
  
  if (Serial.available()){ 
    speed_1 = Serial.parseInt();
    if (speed_1 != old_speed_1) {
      old_speed_1 = speed_1;
      Serial.print("Received: ");
      Serial.print(speed_1);
      Serial.print('\n');
      firstESC.writeMicroseconds(speed_1);
    }
  }
  
  /*
  // Setup the motors
  firstESC.writeMicroseconds(speed_1);
 
 
  if (Serial.available()) 
    speed_1 = Serial.parseInt();
    Serial.print("Received: ");
    Serial.print(speed_1);
    Serial.print('\n');
  
  delay(1000);
  
  */  
}

