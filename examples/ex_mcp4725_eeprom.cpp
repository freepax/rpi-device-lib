#include <iostream>

#include <stdlib.h>

/// mcp4725 headers
#include <mcp4725.h>


int main(int argc, char **argv)
{
    /// check that we have one arg
    if (argc < 2) {
        std::cout << "enter value between 0 and 4095" << std::endl;
        return 0;
    }

    /// resolve arg
    unsigned short value = atoi(argv[1]);
    if (value < 0 || value > 4095) {
        std::cout << "enter value between 0 and 4095" << std::endl;
        return 0;
    }

    /// create a MCP4725 instance
    MCP4725 mcp4725((char*)FirmwareI2CDeviceses::i2c_1);

    /// turn debugging on/off
    mcp4725.setDebug(false);

    /// set the powerdown bits
    if (mcp4725.setPowerDown(Mcp4725Power::PowerNormal) < 0) {
        std::cerr << __func__ << "(): setControl failed" << std::endl;
        mcp4725.closeDevice();
        return 0;
    }

    /// set the control bits
    if (mcp4725.setControl(Mcp4725Config::ConfigDacEeprom) < 0) {
        std::cerr << __func__ << "(): setControl failed" << std::endl;
        mcp4725.closeDevice();
        return 0;
    }

    /// open device
    int status = mcp4725.openDevice();
    if (status < 0) {
        std::cerr << __func__  << "(): failed to open mcp4725"<< std::endl;
        return 0;
    }

    /// write data to the device
    std::cout << "Testing writeDeviceEeprom" << std::endl;
    status = mcp4725.writeDevice(value);
    if (status < 0) {
        std::cerr << __func__  << "(): failed to write mcp4725 (Eeprom)"<< std::endl;
        return 0;
    }
    std::cout << "Testing writeDeviceEeprom SUCCESS" << std::endl;

    /// close device
    mcp4725.closeDevice();

    return 0;
}
