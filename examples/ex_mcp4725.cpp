#include <iostream>

#include <stdlib.h>

/// binary and mcp4725 headers
#include <binary.h>
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
    MCP4725 mcp4725;

    /// turn debuging on
    mcp4725.setDebug(true);

    /// set device
    if (mcp4725.setDevice((char*)FirmwareI2CDeviceses::i2c_1) < 0) {
        std::cerr << __func__ << "(): setDevice failed" << std::endl;
        mcp4725.closeDevice();
        return 0;
    }

    /// set device i2c bus address
    if (mcp4725.setAddress(Mcp4725Addresses::Mcp4725Address0) < 0) {
        std::cerr << __func__ << "(): setDevice failed" << std::endl;
        mcp4725.closeDevice();
        return 0;
    }

    /// print address in binary
    if (true) {
        std::cout << "address " << std::endl;
        Binary binary;
        binary.printByteAsBinary(mcp4725.address());
    }

    /// set the control bits
    if (mcp4725.setControl(Mcp4725Config::ConfigFast1) < 0) {
        std::cerr << __func__ << "(): setControl failed" << std::endl;
        mcp4725.closeDevice();
        return 0;
    }

    /// set powerdown bits
    if (mcp4725.setPowerDown(Mcp4725Power::PowerNormal) < 0) {
        std::cerr << __func__ << "(): setControl failed" << std::endl;
        mcp4725.closeDevice();
        return 0;
    }

    /// open the device
    int status = mcp4725.openDevice();
    if (status < 0) {
        std::cerr << __func__  << "(): failed to open mcp4725"<< std::endl;
        return 0;
    }

    /// write data to the device
    std::cout << "Testing writeDevice" << std::endl;
    status = mcp4725.writeDevice(value);
    if (status < 0) {
        std::cerr << __func__  << "(): failed to write mcp4725"<< std::endl;
        return 0;
    }
    std::cout << "Testing writeDevice SUCCESS" << std::endl;

    /// close the device
    mcp4725.closeDevice();

    return 0;
}
