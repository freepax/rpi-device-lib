#ifndef MCP4725_H
#define MCP4725_H

/// firmware base class
#include <firmware_i2c.h>


namespace Mcp4725Addresses {
static const unsigned char Mcp4725Address0 = 0b01100000;
static const unsigned char Mcp4725Address1 = 0b01100010;
static const unsigned char Mcp4725Address2 = 0b01100100;
static const unsigned char Mcp4725Address3 = 0b01100110;
static const unsigned char Mcp4725Address4 = 0b01101000;
static const unsigned char Mcp4725Address5 = 0b01101010;
static const unsigned char Mcp4725Address6 = 0b01101100;
static const unsigned char Mcp4725Address7 = 0b01101110;
}

namespace Mcp4725Power {
static const unsigned char PowerNormal = 0x00;
static const unsigned char Power1k     = 0x01;
static const unsigned char Power100k   = 0x02;
static const unsigned char Power500k   = 0x03;
}

namespace Mcp4725Config {
static const unsigned char ConfigFast1     = 0x00;
static const unsigned char ConfigFast2     = 0x01;
static const unsigned char ConfigDacOnly   = 0x02;
static const unsigned char ConfigDacEeprom = 0x03;
}

/// Eeprom register bits
static const unsigned char Busy = 0x80;
static const unsigned char Por  = 0x40;
static const unsigned char Pd1  = 0x04;
static const unsigned char Pd0  = 0x02;


class MCP4725 : public Firmware_I2C {
public:
    /// ctor
    MCP4725(char *device = (char*)FirmwareI2CDeviceses::i2c_0, unsigned char address = Mcp4725Addresses::Mcp4725Address0);

    /// dtor
    ~MCP4725() { closeDevice(); }

    /// set i2c bus address
    int setAddress(unsigned char);

    /// set the three control bits
    int setControl(unsigned char control);

    /// retrieve the control bits
    unsigned char control(void) { return mControl; }

    /// set the two powerdown bits
    int setPowerDown(unsigned char power);

    /// retrieve the powerdown bits
    unsigned char powerDown(void) { return mPowerDown; }

    /// write value [0 ... 4095] to dac
    int writeDevice(unsigned short value);

    /// read eeprom (config) and dac value
    int readDevice(unsigned char *config, unsigned short *dac, unsigned char bytes = 3);

    /// print eeprom (config) content resolved by bits
    void resolveConfig(unsigned char config);

private:
    unsigned char mControl;
    unsigned char mPowerDown;
};

#endif /// MCP4725_H
