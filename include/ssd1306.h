#ifndef SSD1306_H
#define SSD1306_H

/// firmware base class
#include <firmware_i2c.h>


#define Ssd1306_128_64


namespace SSD1306Addresses {
static const unsigned char SSD1306Address0 = 0b00111100;
static const unsigned char SSD1306Address1 = 0b00111101;
}

namespace SSD1306Continuation {
static const bool SSD1306ContinuationOff = true;
static const bool SSD1306ContinuationOn  = false;
}

namespace SSD1306Command {
static const bool SSD1306CommandCommand = false;
static const bool SSD1306CommandData    = true;
}



#if defined Ssd1306_128_64 && defined Ssd1306_128_32
#error "Only one display can be specified at once"
#endif
#if !defined Ssd1306_128_64 && !defined Ssd1306_128_32
#error "At least one display must be specified"
#endif

#if defined Ssd1306_128_64
static const unsigned int Ssd1306LcdWitdh   = 128;
static const unsigned int Ssd1306LcdHeight  = 64;
#endif
#if defined Ssd1306_128_32
static const unsigned int Ssd1306LcdWidth   = 128;
static const unsigned int Ssd1306LcdHeight  = 32;
#endif

static const unsigned int Ssd1306SetContrast                        = 0x81;
static const unsigned int Ssd1306_DisplayAllOnResume                = 0xA4;
static const unsigned int Ssd1306_DisplayAllOn                      = 0xA5;
static const unsigned int Ssd1306NormalDisplay                      = 0xA6;
static const unsigned int Ssd1306InvertDisplay                      = 0xA7;
static const unsigned int Ssd1306DisplayOff                         = 0xAE;
static const unsigned int Ssd1306DisplayOn                          = 0xAF;

static const unsigned int Ssd1306SetDisplayOffset                   = 0xD3;
static const unsigned int Ssd1306SetComPins                         = 0xDA;

static const unsigned int Ssd1306SetVComDetect                      = 0xDB;

static const unsigned int Ssd1306SetDisplayClockDiv                 = 0xD5;
static const unsigned int Ssd1306SetPreCharge                       = 0xD9;

static const unsigned int ssd1306SetMultiplex                       = 0xA8;

static const unsigned int ssd1306SetLowColumn                       = 0x00;
static const unsigned int ssd1306SetHighColumn                      = 0x10;

static const unsigned int ssd1306SetStartLine                       = 0x40;

static const unsigned int ssd1306MemoryMode                         = 0x20;
static const unsigned int ssd1306ColumnAddress                      = 0x21;
static const unsigned int ssd1306PageAddress                        = 0x22;

static const unsigned int ssd1306ComScanInc                         = 0xC0;
static const unsigned int ssd1306ComScanDec                         = 0xC8;

static const unsigned int ssd1306SegReMap                           = 0xA0;

static const unsigned int ssd1306ChcargePump                        = 0x8D;

static const unsigned int ssd1306ExternalVcc                        = 0x1;
static const unsigned int ssd1306SwitchCaPVcc                       = 0x2;

static const unsigned int Ssd1306ActivateScroll                     = 0x2F;
static const unsigned int Ssd1306DeactivateScroll                   = 0x2E;
static const unsigned int Ssd1306SetVerticalScrollArea              = 0xA3;
static const unsigned int Ssd1306RightHorizontalScroll              = 0x26;
static const unsigned int Ssd1306LeftHorizontalScroll               = 0x27;
static const unsigned int Ssd1306VerticalAndRrightHhorizontalScroll = 0x29;
static const unsigned int Ssd1306VerticalAndLeftHorizontalScroll    = 0x2A;

static const bool Black = false;
static const bool White = true;

static const bool Lo    = false;
static const bool High  = true;


class SSD1306 : public Firmware_I2C {
public:
    /// ctor
    SSD1306(char *device = (char*)FirmwareI2CDeviceses::i2c_0, unsigned char address = SSD1306Addresses::SSD1306Address0);

    /// dtor
    ~SSD1306() { closeDevice(); }

    int setAddress(unsigned char address);

    int runCommand(unsigned char command);

    int writeData(char *data, int size);


private:
    bool mCo;                               /// Continuation bit 0: data bytes only
    bool mDc;                               /// 0: Data folling byte are command. 1: Following byte are GDDRAM data
};

#endif /// SSD1306_H



#if 0
    volatile uint8_t *mMosiport;
    volatile uint8 *mClkport:
    volatile uint8 *mCsport;
    volatile uint8 *mDcport;

    uint8_t mosipinmask;
    uint8_t clkpinmask;
    uint8_t cspinmask;
    uint8_t dcpinmask;
#endif
