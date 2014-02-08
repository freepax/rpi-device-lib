#ifndef BMP180_H
#define BMP180_H

#include <firmware_i2c.h>


/// nominal pressure at sea level in Pa (not hPa)
static const long P0AtSea = 101325;


/// calculate altitude from give pressure and P0 (both Pa)
double altitude(long pa, long p0);


/// OSS modes
enum OssMode {
    ModeOss0,
    ModeOss1,
    ModeOss2,
    ModeOss3
};


namespace BMP180 {
static const unsigned char I2cAddress               = 0b01110111;

static const unsigned char ControlRegisterAddress   = 0xF4;

static const unsigned char ChipIdAddress            = 0xD0;

static const unsigned char EEPromStartAddress       = 0xAA;
static const unsigned char EEPromEndAddress         = 0xBF;

static const unsigned char ConvertedValueMsb        = 0xF6;
static const unsigned char ConvertedValueLsb        = 0xF7;
static const unsigned char ConvertedValueXLsb       = 0xF8;

static const unsigned char TemperaturConfig         = 0x2E;     /// max conversion time  4.5 ms

static const unsigned char PressureOss0             = 0x34;     /// max conversion time  4.5 ms
static const unsigned char PressureOss1             = 0x74;     /// max conversion time  7.5 ms
static const unsigned char PressureOss2             = 0xB4;     /// max conversion time 13.5 ms
static const unsigned char PressureOss3             = 0xF4;     /// max conversion time 25.5 ms
}


typedef struct bmp_calck_s {
    long UT;
    unsigned long UP;
    long x1;
    long x2;
    long x3;
    long b3;
    unsigned long b4;
    long b5;
    long b6;
    unsigned long b7;
    long p;
} bmp_calck_t;


typedef struct bmp_calibration_s {
    short ac1;
    short ac2;
    short ac3;
    unsigned short ac4;
    unsigned short ac5;
    unsigned short ac6;
    short b1;
    short b2;
    short mb;
    short mc;
    short md;
} bmp_calibration_t;


class Bmp180 : public Firmware_I2C
{
public:
    Bmp180(char *device = (char*)FirmwareI2CDeviceses::i2c_0, unsigned char address = BMP180::I2cAddress);

    unsigned char address() { return mAddress; }

    int openDevice();

    int readChipId();

    int readTemperatur(float *temperatur);
    int readPressure(long *pascal, int oss = ModeOss3, bool update_temperatur = true);


private:
    int readTemperatur();
    int readPressure(int oss, bool update_temperatur);

    bmp_calck_t mCalc;
    bmp_calibration_t mCalibration;
};

#endif /// BMP180_H
