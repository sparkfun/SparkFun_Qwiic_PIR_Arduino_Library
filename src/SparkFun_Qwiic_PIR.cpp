/******************************************************************************
SparkFun_Qwiic_Button.cpp
SparkFun Qwiic Button/Switch Library Source File
Fischer Moseley @ SparkFun Electronics
Original Creation Date: July 24, 2019

This file implements the QwiicPIR class, prototyped in SparkFun_Qwiic_Button.h

Development environment specifics:
	IDE: Arduino 1.8.9
	Hardware Platform: Arduino Uno/SparkFun Redboard
	Qwiic Button Version: 1.0.0
    Qwiic Switch Version: 1.0.0

This code is Lemonadeware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/

#include <Wire.h>
#include <SparkFun_Qwiic_PIR.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/*-------------------------------- Device Status ------------------------*/

bool QwiicPIR::begin(uint8_t address, TwoWire &wirePort)
{
    _deviceAddress = address; //grab the address that the sensor is on
    _i2cPort = &wirePort;     //grab the port that the user wants to use

    //return true if the device is connected and the device ID is what we expect
    return (isConnected() && checkDeviceID());
}

bool QwiicPIR::isConnected()
{
    _i2cPort->beginTransmission(_deviceAddress);
    if (_i2cPort->endTransmission() == 0)
        return true;
    return false;
}

uint8_t QwiicPIR::deviceID()
{
    return readSingleRegister(ID); //read and return the value in the ID register
}

bool QwiicPIR::checkDeviceID()
{
    // return ((deviceID() == DEV_ID_SW) || (deviceID() == DEV_ID_BTN)); //Return true if the device ID matches either the button or the switch
    return (deviceID() == DEV_ID); //Return true if the device ID matches
}

uint8_t QwiicPIR::getDeviceType()
{
    if (isConnected())
    { //only try to get the device ID if the device will acknowledge
        uint8_t id = deviceID();
        if (id == DEV_ID)
            return 1;
        // if (id == DEV_ID_SW)
        //     return 2;
    }
    return 0;
}

uint16_t QwiicPIR::getFirmwareVersion()
{
    uint16_t version = (readSingleRegister(FIRMWARE_MAJOR)) << 8;
    version |= readSingleRegister(FIRMWARE_MINOR);
    return version;
}

bool QwiicPIR::setI2Caddress(uint8_t address)
{
    if (address < 0x08 || address > 0x77)
    {
        return false; //error immediately if the address is out of legal range
    }

    bool success = writeSingleRegister(I2C_ADDRESS, address);

    if (success == true)
    {
        _deviceAddress = address;
        return true;
    }

    else
    {
        return false;
    }
}

uint8_t QwiicPIR::getI2Caddress()
{
    _deviceAddress = readSingleRegister(I2C_ADDRESS);
    return _deviceAddress;
}

/*------------------------------ Button Status ---------------------- */
bool QwiicPIR::rawPIRReading()
{
    statusRegisterBitField statusRegister;
    statusRegister.byteWrapped = readSingleRegister(EVENT_STATUS);
    return statusRegister.rawReading;
}

bool QwiicPIR::objectDetected()
{
    statusRegisterBitField statusRegister;
    statusRegister.byteWrapped = readSingleRegister(EVENT_STATUS);
    return statusRegister.objectDetected;
}

bool QwiicPIR::objectRemoved()
{
    statusRegisterBitField statusRegister;
    statusRegister.byteWrapped = readSingleRegister(EVENT_STATUS);
    return statusRegister.objectRemoved;
}

uint16_t QwiicPIR::getDebounceTime()
{
    return readDoubleRegister(PIR_DEBOUNCE_TIME);
}

uint8_t QwiicPIR::setDebounceTime(uint16_t time)
{
    return writeDoubleRegisterWithReadback(PIR_DEBOUNCE_TIME, time);
}

/*------------------- Interrupt Status/Configuration ---------------- */
uint8_t QwiicPIR::enableDetectInterrupt()
{
    interruptConfigBitField interruptConfigure;
    interruptConfigure.byteWrapped = readSingleRegister(INTERRUPT_CONFIG);
    interruptConfigure.detectedEnable = 1;
    return writeSingleRegisterWithReadback(INTERRUPT_CONFIG, interruptConfigure.byteWrapped);
}

uint8_t QwiicPIR::disableDetectInterrupt()
{
    interruptConfigBitField interruptConfigure;
    interruptConfigure.byteWrapped = readSingleRegister(INTERRUPT_CONFIG);
    interruptConfigure.detectedEnable = 0;
    return writeSingleRegisterWithReadback(INTERRUPT_CONFIG, interruptConfigure.byteWrapped);
}

uint8_t QwiicPIR::enableRemoveInterrupt()
{
    interruptConfigBitField interruptConfigure;
    interruptConfigure.byteWrapped = readSingleRegister(INTERRUPT_CONFIG);
    interruptConfigure.removedEnable = 1;
    return writeSingleRegisterWithReadback(INTERRUPT_CONFIG, interruptConfigure.byteWrapped);
}

uint8_t QwiicPIR::disableRemoveInterrupt()
{
    interruptConfigBitField interruptConfigure;
    interruptConfigure.byteWrapped = readSingleRegister(INTERRUPT_CONFIG);
    interruptConfigure.removedEnable = 0;
    return writeSingleRegisterWithReadback(INTERRUPT_CONFIG, interruptConfigure.byteWrapped);
}

bool QwiicPIR::available()
{
    statusRegisterBitField buttonStatus;
    buttonStatus.byteWrapped = readSingleRegister(EVENT_STATUS);
    return buttonStatus.eventAvailable;
}

uint8_t QwiicPIR::clearEventBits()
{
    statusRegisterBitField buttonStatus;
    buttonStatus.byteWrapped = readSingleRegister(EVENT_STATUS);
    buttonStatus.objectDetected = 0;
    buttonStatus.objectRemoved= 0;
    buttonStatus.eventAvailable = 0;
    return writeSingleRegisterWithReadback(EVENT_STATUS, buttonStatus.byteWrapped);
}

uint8_t QwiicPIR::resetInterruptConfig()
{
    interruptConfigBitField interruptConfigure;
    interruptConfigure.detectedEnable = 1;
    interruptConfigure.removedEnable = 1;
    return writeSingleRegisterWithReadback(INTERRUPT_CONFIG, interruptConfigure.byteWrapped);
    statusRegisterBitField buttonStatus;
    buttonStatus.eventAvailable = 0;
    return writeSingleRegisterWithReadback(EVENT_STATUS, buttonStatus.byteWrapped);
}

/*------------------------- Queue Manipulation ---------------------- */
//detected queue manipulation
bool QwiicPIR::isObjectDetectedQueueFull()
{
    queueStatusBitField detectedQueueStatus;
    detectedQueueStatus.byteWrapped = readSingleRegister(DETECTED_QUEUE_STATUS);
    return detectedQueueStatus.isFull;
}

bool QwiicPIR::isObjectDetectedQueueEmpty()
{
    queueStatusBitField detectedQueueStatus;
    detectedQueueStatus.byteWrapped = readSingleRegister(DETECTED_QUEUE_STATUS);
    return detectedQueueStatus.isEmpty;
}

unsigned long QwiicPIR::timeSinceLastDetect()
{
    return readQuadRegister(DETECTED_QUEUE_FRONT);
}

unsigned long QwiicPIR::timeSinceFirstDetect()
{
    return readQuadRegister(DETECTED_QUEUE_BACK);
}

unsigned long QwiicPIR::popDetectedQueue()
{
    unsigned long tempData = timeSinceFirstDetect(); //grab the oldest value on the queue

    queueStatusBitField detectedQueueStatus;
    detectedQueueStatus.byteWrapped = readSingleRegister(DETECTED_QUEUE_STATUS);
    detectedQueueStatus.popRequest = 1;
    writeSingleRegister(DETECTED_QUEUE_STATUS, detectedQueueStatus.byteWrapped); //remove the oldest value from the queue

    return tempData; //return the value we popped
}

//removed queue manipulation
bool QwiicPIR::isRemovedQueueFull()
{
    queueStatusBitField removedQueueStatus;
    removedQueueStatus.byteWrapped = readSingleRegister(REMOVED_QUEUE_STATUS);
    return removedQueueStatus.isFull;
}

bool QwiicPIR::isRemovedQueueEmpty()
{
    queueStatusBitField removedQueueStatus;
    removedQueueStatus.byteWrapped = readSingleRegister(REMOVED_QUEUE_STATUS);
    return removedQueueStatus.isEmpty;
}

unsigned long QwiicPIR::timeSinceLastRemove()
{
    return readQuadRegister(REMOVED_QUEUE_FRONT);
}

unsigned long QwiicPIR::timeSinceFirstRemove()
{
    return readQuadRegister(REMOVED_QUEUE_BACK);
}

unsigned long QwiicPIR::popRemovedQueue()
{
    unsigned long tempData = timeSinceFirstRemove();
    queueStatusBitField removedQueueStatus;
    removedQueueStatus.byteWrapped = readSingleRegister(REMOVED_QUEUE_STATUS);
    removedQueueStatus.popRequest = 1;
    writeSingleRegister(REMOVED_QUEUE_STATUS, removedQueueStatus.byteWrapped);
    return tempData;
}

/*------------------------- Internal I2C Abstraction ---------------- */

uint8_t QwiicPIR::readSingleRegister(Qwiic_Button_Register reg)
{
    _i2cPort->beginTransmission(_deviceAddress);
    _i2cPort->write(reg);
    _i2cPort->endTransmission();

    //typecasting the 1 parameter in requestFrom so that the compiler
    //doesn't give us a warning about multiple candidates
    if (_i2cPort->requestFrom(_deviceAddress, static_cast<uint8_t>(1)) != 0)
    {
        return _i2cPort->read();
    }
    return 0;
}

uint16_t QwiicPIR::readDoubleRegister(Qwiic_Button_Register reg)
{ //little endian
    _i2cPort->beginTransmission(_deviceAddress);
    _i2cPort->write(reg);
    _i2cPort->endTransmission();

    //typecasting the 2 parameter in requestFrom so that the compiler
    //doesn't give us a warning about multiple candidates
    if (_i2cPort->requestFrom(_deviceAddress, static_cast<uint8_t>(2)) != 0)
    {
        uint16_t data = _i2cPort->read();
        data |= (_i2cPort->read() << 8);
        return data;
    }
    return 0;
}

unsigned long QwiicPIR::readQuadRegister(Qwiic_Button_Register reg)
{
    _i2cPort->beginTransmission(_deviceAddress);
    _i2cPort->write(reg);
    _i2cPort->endTransmission();

    union databuffer {
        uint8_t array[4];
        unsigned long integer;
    };

    databuffer data;

    //typecasting the 4 parameter in requestFrom so that the compiler
    //doesn't give us a warning about multiple candidates
    if (_i2cPort->requestFrom(_deviceAddress, static_cast<uint8_t>(4)) != 0)
    {
        for (uint8_t i = 0; i < 4; i++)
        {
            data.array[i] = _i2cPort->read();
        }
    }
    return data.integer;
}

bool QwiicPIR::writeSingleRegister(Qwiic_Button_Register reg, uint8_t data)
{
    _i2cPort->beginTransmission(_deviceAddress);
    _i2cPort->write(reg);
    _i2cPort->write(data);
    if (_i2cPort->endTransmission() == 0)
        return true;
    return false;
}

bool QwiicPIR::writeDoubleRegister(Qwiic_Button_Register reg, uint16_t data)
{
    _i2cPort->beginTransmission(_deviceAddress);
    _i2cPort->write(reg);
    _i2cPort->write(lowByte(data));
    _i2cPort->write(highByte(data));
    if (_i2cPort->endTransmission() == 0)
        return true;
    return false;
}

uint8_t QwiicPIR::writeSingleRegisterWithReadback(Qwiic_Button_Register reg, uint8_t data)
{
    if (writeSingleRegister(reg, data))
        return 1;
    if (readSingleRegister(reg) != data)
        return 2;
    return 0;
}

uint16_t QwiicPIR::writeDoubleRegisterWithReadback(Qwiic_Button_Register reg, uint16_t data)
{
    if (writeDoubleRegister(reg, data))
        return 1;
    if (readDoubleRegister(reg) != data)
        return 2;
    return 0;
}