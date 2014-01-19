#ifndef L3GD20_H
#define L3GD20_H

/// linux i2c driver headers
#include <firmware_i2c.h>


/// two possible i2c addresses for L3GD20
namespace L3dg20 {
static const int L3gd20Address0 = 0b01101010;
static const int L3gd20Address1 = 0b01101011;
}


/// the L3GD20 register addresses
namespace L3dg20RegistersAddresses {
static const int mWhoAddress                     = 0b00001111;
static const int mControl_1Address               = 0b00100000;
static const int mControl_2Address               = 0b00100001;
static const int mControl_3Address               = 0b00100010;
static const int mControl_4Address               = 0b00100011;
static const int mControl_5Address               = 0b00100100;
static const int mReferenceAddress               = 0b00100101;
static const int mTemperaturAddress              = 0b00100110;
static const int mStatusAddress                  = 0b00100111;
static const int mOutXLAddress                   = 0b00101000;
static const int mOutXHAddress                   = 0b00101001;
static const int mOutYLAddress                   = 0b00101010;
static const int mOutYHAddress                   = 0b00101011;
static const int mOutZLAddress                   = 0b00101100;
static const int mOutZHAddress                   = 0b00101101;
static const int mFifoControlAddress             = 0b00101110;
static const int mFifoControlSourceAddress       = 0b00101111;
static const int mInteruptConfigureAddress       = 0b00110000;
static const int mInteruptConfigureSourceAddress = 0b00110001;
static const int mInteruptXHAddress              = 0b00110010;
static const int mInteruptXLAddress              = 0b00110011;
static const int mInteruptYHAddress              = 0b00110100;
static const int mInteruptYLAddress              = 0b00110101;
static const int mInteruptzHAddress              = 0b00110110;
static const int mInteruptzlAddress              = 0b00110111;
static const int mInteruptDurationAddress        = 0b00111000;
}


/// the L3GD20 register defaults
namespace L3dg20RegistersDefaults {
static const int mWhoDefaults                     = 0b11010100;
static const int mControl_1Defaults               = 0b00000111;
static const int mControl_2Defaults               = 0b00000000;
static const int mControl_3Defaults               = 0b00000000;
static const int mControl_4Defaults               = 0b00000000;
static const int mControl_5Defaults               = 0b00000000;
static const int mReferenceDefaults               = 0b00000000;
static const int mFifoControlDefaults             = 0b00000000;
static const int mInteruptConfigureDefaults       = 0b00000000;
static const int mInteruptXHDefaults              = 0b00000000;
static const int mInteruptXLDefaults              = 0b00000000;
static const int mInteruptYHDefaults              = 0b00000000;
static const int mInteruptYLDefaults              = 0b00000000;
static const int mInteruptzHDefaults              = 0b00000000;
static const int mInteruptzlDefaults              = 0b00000000;
static const int mInteruptDurationDefaults        = 0b00000000;
}

static const char *I2C_DEVICE = "/dev/i2c-1";


class L3GD20 : public Firmware_I2C {
public:
    /// ctor
    L3GD20(char *device = (char*)FirmwareI2CDeviceses::i2c_0, int address = L3dg20::L3gd20Address0, bool debug = false);

    /// dtor
    ~L3GD20();

    /// set and get i2c address
    int setAddress(int);


private:
    /// L3GD20 REGISTERS
    int mWho;

    int mControl_1;
    int mControl_2;
    int mControl_3;
    int mControl_4;
    int mControl_5;

    int mReference;

    int mTemperatur;

    int mStatus;

    int mOutXL;
    int mOutXH;
    int mOutYL;
    int mOutYH;
    int mOutZL;
    int mOutZH;

    int mFifoControl;
    int mFifoControlSource;

    int mInteruptConfigure;
    int mInteruptConfigureSource;

    int mInteruptXH;
    int mInteruptXL;
    int mInteruptYH;
    int mInteruptYL;
    int mInteruptzH;
    int mInteruptzl;

    int mInteruptDuration;
};

#endif /// L3GD20_H
