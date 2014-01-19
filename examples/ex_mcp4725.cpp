#include <iostream>

#include <stdlib.h>

#include <binary.h>
#include <mcp4725.h>


int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cout << "enter value between 0 and 4095" << std::endl;
        return 0;
    }

    int value = atoi(argv[1]);
    if (value < 0 || value > 4095) {
        std::cout << "enter value between 0 and 4095" << std::endl;
        return 0;
    }

    /// create and MCP4725 instance - make sure to use the right device ("/dev/i2c-1")
    //MCP4725 mcp4725((char*)FirmwareI2CDeviceses::i2c_1);
    MCP4725 mcp4725;
    mcp4725.setDevice((char*)FirmwareI2CDeviceses::i2c_1);

    if (0) {
        std::cout << "address " << std::endl;
        Binary binary;
        binary.printByteAsBinary(mcp4725.address());
    }

    int status = mcp4725.openDevice();
    if (status < 0) {
        std::cerr << __func__  << "(): failed to open mcp4725"<< std::endl;
        return 0;
    }

    std::cout << "Testing writeDevice" << std::endl;
    status = mcp4725.writeDevice(value);
    if (status < 0) {
        std::cerr << __func__  << "(): failed to write mcp4725"<< std::endl;
        return 0;
    }
    std::cout << "Testing writeDevice SUCCESS" << std::endl;

    return 0;
}
