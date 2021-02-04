/******************************************************************************
Shows how to use the FIFO Queue on the Qwiic PIR

Fischer Moseley @ SparkFun Electronics
Original Creation Date: July 29, 2019
Revised by Andy England, 1/5/2021
This code is Lemonadeware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Hardware Connections:
Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
Plug the PIR into the shield
Print it to the serial monitor at 115200 baud.

Distributed as-is; no warranty is given.
******************************************************************************/

#include <SparkFun_Qwiic_PIR.h>
QwiicPIR pir;

void setup(){
    Serial.begin(115200);
    Serial.println("Qwiic PIR examples");
    Wire.begin(); //Join I2C bus

  //check if PIR will acknowledge over I2C
  if (pir.begin() == false){
    Serial.println("Device did not acknowledge! Freezing.");
    while(1); 
  }
  Serial.println("PIR acknowledged.");
}

void loop(){
    if(pir.isDetectedQueueEmpty() == false) { //if the queue of pressed events is not empty
        //then print the time since the last and first PIR press
        Serial.print(pir.timeSinceLastDetect()/1000.0);
        Serial.print("s since PIR detect   ");
        Serial.print(pir.timeSinceFirstDetect()/1000.0);
        Serial.print("s since PIR detect   ");
    }

    //if the queue of pressed events is empty, just print that the queue is empty!
    if(pir.isDetectedQueueEmpty() == true) {
        Serial.print("PIR Detected Queue is empty! ");
    } 

    if(pir.isRemovedQueueEmpty() == false) { //if the queue of clicked events is not empty
        //then print the time since the last and first PIR click
        Serial.print(pir.timeSinceLastRemove()/1000.0);
        Serial.print("s since PIR remove   ");
        Serial.print(pir.timeSinceFirstRemove()/1000.0);
        Serial.print("s since PIR remove   ");
    }
    //if the queue of clicked events is empty, just print that the queue is empty!
    if(pir.isDetectedQueueEmpty() == true) {
        Serial.print("  PIR Removed Queue is empty!");
    }

    Serial.println(); //print a new line to not clutter up the serial monitor

    if(Serial.available()) { //if the user sent a character
        
        uint8_t data = Serial.read();
        if(data == 'd' || data == 'D') { //if the character is p or P, then pop a value off of the detect Queue
            pir.popDetectedQueue();
            Serial.println("Popped DetectedQueue!");
        }

        if(data == 'r' || data == 'R') { //if the character is c or C, then pop a value off of the removed Queue
            pir.popRemovedQueue();
            Serial.println("Popped RemovedQueue!");
        }
    }
    delay(20); //let's not hammer too hard on the I2C bus
}
