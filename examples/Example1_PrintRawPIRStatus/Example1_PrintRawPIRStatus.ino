/******************************************************************************
  This example prints out the raw reading of SparkFun's Qwiic PIR Breakout.

  Andy England @ SparkFun Electronics
  Original Creation Date: January 5, 2021

  This code is Lemonadeware; if you see me (or any other SparkFun employee) at the
  local, and you've found our code helpful, please buy us a round!

  Distributed as-is; no warranty is given.
******************************************************************************/

#include <SparkFun_Qwiic_PIR.h>
QwiicPIR pir;

#define DEBOUNCE_TIME 750

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
}

void loop() {
  //check if pir detects an object, and tell us if it is!
  if (pir.rawPIRReading() == true) {
    Serial.println("Object Detected");
    while (pir.rawPIRReading() == true)
      delay(DEBOUNCE_TIME);  //wait for output signal to stabilize
    Serial.println("Object Removed");
  }
  delay(DEBOUNCE_TIME); //Don't hammer too hard on the I2C bus
}
