/******************************************************************************
  Attaches a hardware interrupt to a pin to tell us when events are available to read.

  Fischer Moseley @ SparkFun Electronics
  Original Creation Date: June 28, 2019

  This code is Lemonadeware; if you see me (or any other SparkFun employee) at the
  local, and you've found our code helpful, please buy us a round!

  Hardware Connections:
  Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
  Plug the pir into the shield
  Print it to the serial monitor at 115200 baud.

  Distributed as-is; no warranty is given.
******************************************************************************/

#include <SparkFun_Qwiic_PIR.h>
QwiicPIR pir;
int interruptPin = 2; //pin that will change states when interrupt is triggered

bool interruptEntered = false; //Interrupt flag so we can clear things outside of the interrupt

void setup() {
  Serial.begin(115200);
  Serial.println("Qwiic PIR examples");

  Wire.begin(); //Join I2C bus

  //intialize interrupt pin
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), pirHandler, FALLING);

  //check if pir will acknowledge over I2C
  if (pir.begin() == false) {
    Serial.println("Device did not acknowledge! Freezing.");
    while (1);
  }

  //Wait for PIR to stabilize
  Serial.println("PIR acknowledged. Waiting 30 seconds to stabilize");
  for (uint8_t seconds = 0; seconds < 30; seconds++)
  {
    Serial.println(seconds);
    delay(1000);
  }

  pir.enableInterrupt();  //configure the interrupt pin to go low when we detect an object with the PIR's view.
  pir.clearEventBits();  //once event bits are cleared, interrupt pin goes high
}

void loop() {
  if (interruptEntered)
  {
    if (pir.objectDetected()) {
      Serial.println("Object Detected");
    }
    if (pir.objectRemoved()) {
      Serial.println("Object Removed");
    }
    pir.clearEventBits();
    interruptEntered = false;
    delay(10);
  }
}

void pirHandler()
{
  interruptEntered = true;
}