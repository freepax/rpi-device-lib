#include <iostream>
#include <stdlib.h>

#include <mcp4725.h>


int main(int argc, char **argv)
{
    /// create and MCP4725 instance - make sure to use the right device ("/dev/i2c-1")
    MCP4725 mcp4725((char*)FirmwareI2CDeviceses::i2c_1);

    int status = mcp4725.openDevice();
    if (status < 0) {
        std::cerr << __func__  << "(): failed to open mcp4725"<< std::endl;
        return 0;
    }

    status = mcp4725.readDevice();
    if (status < 0) {
        std::cerr << __func__  << "(): failed to read mcp4725"<< std::endl;
        return 0;
    }

    std::cout << "MCP3725 status" << std::endl;
    mcp4725.status();

    return 0;
}
