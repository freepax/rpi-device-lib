#ifndef PCA9685_H
#define PCA9685_H

#include <firmware_i2c.h>


/**
 * Mode 1 Register              [ RESTART EXTCLK AI SLEEP SUB1 SUB2 SUB3 ALLCALL ]
 *      RESTART                 Shows state of restart logic.
 *      EXTCLK                  In order to use the extclk pin, this bit must be set (see instructions)
 *      AI                      Register auto increment disable
 *      SLEEP                   Normal mode
 *      SUB 1, 2, 3             Respond to sub address 1, 2 and 3 repectively
 *      ALLCALL                 Respond to LED All Call i2c addresses
 *
 * Mode 2 Register              [ XXX XXX XXX INVRT OCH OUTDRV OUTNE1 OUTNE0 ]
 *      INVRT                   Invert output logic state
 *      OCH                     Output change on STOP command
 *      OUTDRV                  The 16 LEDn outputs are configured with an open-drain if not set, or totem pole if set
 *      OUTNE1 OUTNE0           See doc...
 *
 */

namespace PCA9685Addresses {
static const unsigned char PCA9685Address0  = 0b01000000;
static const unsigned char PCA9685Address1  = 0b01000001;
static const unsigned char PCA9685Address2  = 0b01000010;
static const unsigned char PCA9685Address3  = 0b01000011;

static const unsigned char PCA9685Address4  = 0b01000100;
static const unsigned char PCA9685Address5  = 0b01000101;
static const unsigned char PCA9685Address6  = 0b01000110;
static const unsigned char PCA9685Address7  = 0b01000111;

static const unsigned char PCA9685Address8  = 0b01001000;
static const unsigned char PCA9685Address9  = 0b01001001;
static const unsigned char PCA9685Address10 = 0b01001010;
static const unsigned char PCA9685Address11 = 0b01001011;

static const unsigned char PCA9685Address12 = 0b01001100;
static const unsigned char PCA9685Address13 = 0b01001101;
static const unsigned char PCA9685Address14 = 0b01001110;
static const unsigned char PCA9685Address15 = 0b01001111;
}

namespace PCA9685Constants {
static const unsigned int Off = 0;
static const unsigned int On = 1;
static const unsigned int Lo = 0;
static const unsigned int Hi = 1;
}

namespace PCA9685Registers {
static const unsigned char Mode1          = 0b00000000;
static const unsigned char Mode2          = 0b00000001;
static const unsigned char SubAdress1     = 0b00000010;
static const unsigned char SubAdress2     = 0b00000011;
static const unsigned char SubAdress3     = 0b00000100;
static const unsigned char AllCallAddress = 0b00000101;

static const unsigned char Led0OnLo       = 0b00000110;
static const unsigned char Led0OnHi       = 0b00000111;
static const unsigned char Led0OffLo      = 0b00001000;
static const unsigned char Led0OffHi      = 0b00001001;

static const unsigned char Led1OnLo       = 0b00001010;
static const unsigned char Led1OnHi       = 0b00001011;
static const unsigned char Led1OffLo      = 0b00001100;
static const unsigned char Led1OffHi      = 0b00001101;


static const unsigned char Led2OnLo       = 0b00001110;
static const unsigned char Led2OnHi       = 0b00001111;
static const unsigned char Led2OffLo      = 0b00010000;
static const unsigned char Led2OffHi      = 0b00010001;

static const unsigned char Led3OnLo       = 0b00010010;
static const unsigned char Led3OnHi       = 0b00010011;
static const unsigned char Led3OffLo      = 0b00010100;
static const unsigned char Led3OffHi      = 0b00010101;

static const unsigned char Led4OnLo       = 0b00010110;
static const unsigned char Led4OnHi       = 0b00010111;
static const unsigned char Led4OffLo      = 0b00011000;
static const unsigned char Led4OffHi      = 0b00011001;

static const unsigned char Led5OnLo       = 0b00011010;
static const unsigned char Led5OnHi       = 0b00011011;
static const unsigned char Led5OffLo      = 0b00011100;
static const unsigned char Led5OffHi      = 0b00011101;

static const unsigned char Led6OnLo       = 0b00011110;
static const unsigned char Led6OnHi       = 0b00011111;
static const unsigned char Led6OffLo      = 0b00100000;
static const unsigned char Led6OffHi      = 0b00100001;

static const unsigned char Led7OnLo       = 0b00100010;
static const unsigned char Led7OnHi       = 0b00100011;
static const unsigned char Led7OffLo      = 0b00100100;
static const unsigned char Led7OffHi      = 0b00100101;

static const unsigned char Led8OnLo       = 0b00100110;
static const unsigned char Led8OnHi       = 0b00100111;
static const unsigned char Led8OffLo      = 0b00101000;
static const unsigned char Led8OffHi      = 0b00101001;

static const unsigned char Led9OnLo       = 0b00101010;
static const unsigned char Led9OnHi       = 0b00101011;
static const unsigned char Led9OffLo      = 0b00101100;
static const unsigned char Led9OffHi      = 0b00101101;

static const unsigned char Prescale       = 0b11111110;

}

static const int OutPuts = 16;
static const unsigned int DefaultOscFreq = 25000000;

typedef struct Output {
    int ledOnLo;
    int LedOnHi;
    int ledOffLo;
    int ledOffHi;
} Output_s;


class PCA9685 : public Firmware_I2C {
public:
    PCA9685(char *device = (char*)FirmwareI2CDeviceses::i2c_0, int address = PCA9685Addresses::PCA9685Address0, bool debug = false);

    int setAddress(unsigned char);

    int writeOutput(unsigned char output, int onoff, int hilo, unsigned char value);
    int writeOutput(unsigned char output, unsigned short ontime, unsigned short offtime);


    int setFrequency(unsigned int frequency);
    int setOscillatorFrequency(unsigned int frequency) { mOscillatorFrequency = frequency * 1000000; return 0; }
    unsigned int oscillatoryFrequency(void) { return mOscillatorFrequency; }


private:
    unsigned char mMode1;       /// [ RESTART EXTCLK AI SLEEP SUB1 SUB2 SUB3 ALLCALL ]
    unsigned char mMode2;       /// [ XXX XXX XXX INVRT OCH OUTDRV OUTNE1 OUTNE0 ]

    int mLedAllCall;            ///

    Output ouput[OutPuts];      ///
    Output allOnOff;            ///

    int mPrescale;              ///
    int mOscillatorFrequency;
    int mTestMode;              ///
};

#endif /// PCA9685_H
