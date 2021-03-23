/******************************************************************************
SparkFun_Qwiic_PIR.h
SparkFun Qwiic PIR Library Header File
Andy England @ SparkFun Electronics
Original Creation Date: January 5, 2021
https://github.com/sparkfun/

This file prototypes the QwiicPIR class, implemented in SparkFun_Qwiic_PIR.cpp.

Development environment specifics:
	IDE: Arduino 1.8.9
	Hardware Platform: Arduino Uno/SparkFun Redboard
	Qwiic PIR Breakout Version: 1.0.0

This code is Lemonadeware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/

#ifndef __SparkFun_Qwiic_PIR_H__
#define __SparkFun_Qwiic_PIR_H__

#include <Wire.h>
#include <Arduino.h>
#include "registers.h"

#define DEFAULT_ADDRESS 0x12 //default I2C address of the Qwiic PIR
#define DEV_ID 0x72         //device ID of the Qwiic PIR

class QwiicPIR
{
private:
    TwoWire *_i2cPort;      //Generic connection to user's chosen I2C port
    uint8_t _deviceAddress; //I2C address of the Qwiic PIR

public:
    //Device status
    bool begin(uint8_t address = DEFAULT_ADDRESS, TwoWire &wirePort = Wire); //Sets device I2C address to a user-specified address, over whatever port the user specifies.
    bool isConnected();                                                      //Returns true if the Qwiic PIR will acknowledge over I2C, and false otherwise
    uint8_t deviceID();                                                      //Return the 8-bit device ID of the attached device.
    bool checkDeviceID();                                                    //Returns true if the device ID matches the Qwiic PIR
    uint8_t getDeviceType();                                                 //Returns 1 if a PIR is attached. Returns 0 if there is no device attached.
    uint16_t getFirmwareVersion();                                           //Returns the firmware version of the attached device as a 16-bit integer. The leftmost (high) byte is the major revision number, and the rightmost (low) byte is the minor revision number.
    bool setI2Caddress(uint8_t address);                                     //Configures the attached device to attach to the I2C bus using the specified address
    uint8_t getI2Caddress();                                                 //Returns the I2C address of the device.

    //PIR status/config
    bool rawPIRReading();                   //Returns 1 when the PIR is outputting a 1 and 0 when not. This is the raw output from the PIR with no debouncing
    bool objectDetected();                  //Returns 1 if a debounced object detection event occurs
    bool objectRemoved();                   //Returns 1 if a debounced object removal event occurs.
    uint16_t getDebounceTime();             //Returns the time to wait for the PIR signal to settle, (in milliseconds).
    uint8_t setDebounceTime(uint16_t time); //Sets the time to wait for the PIR signal to settle (in milliseconds) and checks if the register was set properly. Returns 0 on success, 1 on register I2C write fail, and 2 if the value didn't get written into the register properly.

    //Interrupt status/config
    uint8_t enableInterrupt();  //When called, the interrupt will be configured to trigger when an object is detected by the PIR. 
    uint8_t disableInterrupt(); //When called, the interrupt will no longer be configured to trigger when an object is detected by the PIR. 
    bool available();                  //Returns the eventAvailable bit
    uint8_t clearEventBits();          //Sets objectDetected, objectRemoved, and eventAvailable to zero
    uint8_t resetInterruptConfig();    //Resets the interrupt configuration back to defaults.

    //Queue manipulation
    bool isDetectedQueueFull();           //Returns true if the queue of object detection timestamps is full, and false otherwise.
    bool isDetectedQueueEmpty();          //Returns true if the queue of object detection timestamps is empty, and false otherwise.
    unsigned long timeSinceLastDetect();  //Returns how many milliseconds it has been since the last object detection. Since this returns a 32-bit unsigned int, it will roll over about every 50 days.
    unsigned long timeSinceFirstDetect(); //Returns how many milliseconds it has been since the first object detection. Since this returns a 32-bit unsigned int, it will roll over about every 50 days.
    unsigned long popDetectedQueue();     //Returns the oldest value in the queue (milliseconds since first object detection), and then removes it.

    bool isRemovedQueueFull();           //Returns true if the queue of object removal timestamps is full, and false otherwise.
    bool isRemovedQueueEmpty();          //Returns true if the queue of object removal timestamps is empty, and false otherwise.
    unsigned long timeSinceLastRemove();  //Returns how many milliseconds it has been since the last object removal. Since this returns a 32-bit unsigned int, it will roll over about every 50 days.
    unsigned long timeSinceFirstRemove(); //Returns how many milliseconds it has been since the first object removal. Since this returns a 32-bit unsigned int, it will roll over about every 50 days.
    unsigned long popRemovedQueue();     //Returns the oldest value in the queue (milliseconds since first object removal), and then removes it.

    //Internal I2C Abstraction
    uint8_t readSingleRegister(Qwiic_PIR_Register reg);                              //Reads a single 8-bit register.
    uint16_t readDoubleRegister(Qwiic_PIR_Register reg);                             //Reads a 16-bit register (little endian).
    unsigned long readQuadRegister(Qwiic_PIR_Register reg);                          //Reads a 32-bit register (little endian).
    bool writeSingleRegister(Qwiic_PIR_Register reg, uint8_t data);                  //Attempts to write data into a single 8-bit register. Does not check to make sure it was written successfully. Returns 0 if there wasn't an error on I2C transmission, and 1 otherwise.
    bool writeDoubleRegister(Qwiic_PIR_Register reg, uint16_t data);                 //Attempts to write data into a double (two 8-bit) registers. Does not check to make sure it was written successfully. Returns 0 if there wasn't an error on I2C transmission, and 1 otherwise.
    uint8_t writeSingleRegisterWithReadback(Qwiic_PIR_Register reg, uint8_t data);   //Writes data into a single 8-bit register, and checks to make sure that the data was written successfully. Returns 0 on no error, 1 on I2C write fail, and 2 if the register doesn't read back the same value that was written.
    uint16_t writeDoubleRegisterWithReadback(Qwiic_PIR_Register reg, uint16_t data); //Writes data into a double (two 8-bit) registers, and checks to make sure that the data was written successfully. Returns 0 on no error, 1 on I2C write fail, and 2 if the register doesn't read back the same value that was written.
};
#endif