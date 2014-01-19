#ifndef MCP4725_H
#define MCP4725_H

/// linux i2c driver headers
#include <firmware_i2c.h>


namespace Mcp4725Addresses {
static const unsigned char Mcp4725Address0 = 0b01100010;
static const unsigned char Mcp4725Address1 = 0b01100011;
}


class MCP4725 : public Firmware_I2C {
public:
    /// ctor
    MCP4725(char *device = (char*)FirmwareI2CDeviceses::i2c_0, unsigned char address = Mcp4725Addresses::Mcp4725Address0, bool debug = false);

    /// dtor
    ~MCP4725();

    int writeDevice(float voltage);
    int writeDevice(int value);

    int writeDeviceEeprom(float voltage);
    int writeDeviceEeprom(int value);

    int readDevice();
    void status(void);

    int setAddress(unsigned char);


private:

    /// the config register
    int mConfig;
};

#endif /// MCP4725_H
