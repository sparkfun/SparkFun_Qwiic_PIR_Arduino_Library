/******************************************************************************
  Checks whether the PIR is detecting an object

  Andy England @ SparkFun Electronics
  Original Creation Date: January 5, 2021

  This code is Lemonadeware; if you see me (or any other SparkFun employee) at the
  local, and you've found our code helpful, please buy us a round!

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

  Serial.println("PIR warm!");

  //Use this function call to change the debounce time of the PIR sensor
  //The parameter is the debounce time in milliseconds
  pir.setDebounceTime(500);
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
