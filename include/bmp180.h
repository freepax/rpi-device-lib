#ifndef BMP180_H
#define BMP180_H

#include <firmware_i2c.h>


namespace BMP180 {
static const unsigned char I2cAddress               = 0b01110111;

static const unsigned char ControlRegisterAddress   = 0xF4;
static const unsigned char EEPromStartAddress       = 0xAA;
static const unsigned char EEPromEndAddress         = 0xBF;
static const unsigned char ConvertedValueLsb        = 0xF7;
static const unsigned char ConvertedValueMsb        = 0xF6;
static const unsigned char ConvertedValueXLsb       = 0xF8;

static const unsigned char TemperaturConfig         = 0x2E;     /// max conversion time  4.5 ms
static const unsigned char PressureOss0             = 0x34;     /// max conversion time  4.5 ms
static const unsigned char PressureOss1             = 0x74;     /// max conversion time  7.5 ms
static const unsigned char PressureOss2             = 0xB4;     /// max conversion time 13.5 ms
static const unsigned char PressureOss3             = 0xF4;     /// max conversion time 25.5 ms
}

static const long P0AtSea = 101325;

double altitude(long pa, long p0);


enum OssMode {
    ModeOss0,
    ModeOss1,
    ModeOss2,
    ModeOss3
};


typedef struct bmp_calck_t {
    long UT;
    long UP;
    long x1;
    long x2;
    long x3;
    long b3;
    unsigned long b4;
    long b5;
    long b6;
    unsigned long b7;
    long p;
} bmp_calck_s;

class Bmp180 : public Firmware_I2C
{
public:
    Bmp180(char *device = (char*)FirmwareI2CDeviceses::i2c_0, unsigned char address = BMP180::I2cAddress);

    int setResolution();
    int initialize();

    int readTemperatur();
    float temperatur() { return mTemperatur; }
    long pressure() const { return mPressure; }
    long readPressure(int mode = ModeOss0, bool readtemp = true);

    unsigned char address() { return mAddress; }

private:
    short mAc1;
    short mAc2;
    short mAc3;
    unsigned short mAc4;
    unsigned short mAc5;
    unsigned short mAc6;
    short mB1;
    short mB2;
    short mMb;
    short mMc;
    short mMd;

    float mTemperatur;
    long mPressure;

    bmp_calck_s mCalc;
};

#endif /// BMP180_H
