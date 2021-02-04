/******************************************************************************
registers.h
Fischer Moseley @ SparkFun Electronics
Original Creation Date: July 24, 2019

This file defines the virtual memory map on the Qwiic Button/Switch. The enum
provides a set of pointers for the various registers on the Qwiic
Button/Switch, and the unions wrap the various bits in the bitfield in an easy
to use uint8_t format

Development environment specifics:
	IDE: Arduino 1.8.9
	Hardware Platform: Arduino Uno/SparkFun Redboard
	Qwiic Button Version: 1.0.0
    Qwiic Switch Version: 1.0.0

This code is Lemonadeware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/

//Register Pointer Map
enum Qwiic_Button_Register : uint8_t
{
    ID = 0x00,
    FIRMWARE_MINOR = 0x01,
    FIRMWARE_MAJOR = 0x02,
    EVENT_STATUS = 0x03,
    INTERRUPT_CONFIG = 0x04,
    PIR_DEBOUNCE_TIME = 0x05,
    DETECTED_QUEUE_STATUS = 0x07,
    DETECTED_QUEUE_FRONT = 0x08,
    DETECTED_QUEUE_BACK = 0x0C,
    REMOVED_QUEUE_STATUS = 0x10,
    REMOVED_QUEUE_FRONT = 0x11,
    REMOVED_QUEUE_BACK = 0x15,
    I2C_ADDRESS = 0x19
};

typedef union {
    struct
    {
        bool rawReading : 1; //This is the raw reading from the PIR, it is not user writable
        bool eventAvailable : 1; //This is bit 0. User mutable, gets set to 1 when a new event occurs. User is expected to write 0 to clear the flag.
        bool objectRemoved: 1; //Defaults to zero on POR. Gets set to one when the button gets clicked. Must be cleared by the user.
        bool objectDetected : 1;      //Gets set to one if button is pushed.
        bool : 4;
    };
    uint8_t byteWrapped;
} statusRegisterBitField;

typedef union {
    struct
    {
        bool interruptEnable : 1; //This is bit 0. user mutable, set to 1 to enable an interrupt when the button is clicked
        bool : 7;
    };
    uint8_t byteWrapped;
} interruptConfigBitField;

typedef union {
    struct
    {
        bool popRequest : 1; //This is bit 0. User mutable, user sets to 1 to pop from queue, we pop from queue and set the bit back to zero.
        bool isEmpty : 1;    //user immutable, returns 1 or 0 depending on whether or not the queue is empty
        bool isFull : 1;     //user immutable, returns 1 or 0 depending on whether or not the queue is full
        bool : 5;
    };
    uint8_t byteWrapped;
} queueStatusBitField;