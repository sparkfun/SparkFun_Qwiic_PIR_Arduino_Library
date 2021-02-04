/******************************************************************************
  Checks whether the PIR is detecting an object

  Fischer Moseley @ SparkFun Electronics
  Original Creation Date: June 28, 2019
  Revised by Andy England, 1/5/2021

  This code is Lemonadeware; if you see me (or any other SparkFun employee) at the
  local, and you've found our code helpful, please buy us a round!

  Hardware Connections:
  Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
  Plug the button into the shield
  Print it to the serial monitor at 115200 baud.

  Distributed as-is; no warranty is given.
******************************************************************************/

#include <SparkFun_Qwiic_PIR.h>
QwiicPIR pir;

void setup() {
  Serial.begin(115200);
  Serial.println("Qwiic PIR examples");
  Wire.begin(); //Join I2C bus

  //check if pir will acknowledge over I2C
  if (pir.begin() == false) {
    Serial.println("Device did not acknowledge! Freezing.");
    while (1);
  }
  
  Serial.println("PIR acknowledged. Waiting 30 Seconds while PIR warms up");
  for (uint8_t seconds = 0; seconds < 30; seconds++)
  {
    Serial.println(seconds);
    delay(1000);
  }
  
}

void loop() {
  //check if there is an available PIR event, and tell us what it is!
  if (pir.available()) {
    if (pir.objectDetected()) {
      Serial.println("Object Detected");
    }    
    if (pir.objectRemoved()) {
      Serial.println("Object Removed");
    }
    pir.clearEventBits();
  }
}