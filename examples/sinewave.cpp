#include <iostream>

#include <unistd.h>

/// sinewave and mcp4725 headers
#include <sinewave.h>
#include <mcp4725.h>


int main(int argc, char **argv)
{
    int i;

    /// create and MCP4725 instance - make sure to use the right device ("/dev/i2c-1")
    MCP4725 mcp4725((char*)FirmwareI2CDeviceses::i2c_1);

    //mcp4725.setDebug(true);

    if (mcp4725.setAddress(Mcp4725Addresses::Mcp4725Address1) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " setAddress failed" << std::endl;
        return 0;
    }

    if (mcp4725.openDevice() < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " failed to open mcp4725 device" << std::endl;
        return 0;
    }

    while ( 1 ) {
        for (i = 0; i < 512; i++) {
            /// write next byte in sinewave pattern
            if (mcp4725.writeDevice(SineWave::Sinewave[i]) < 0) {
                std::cerr << __func__ << ":" << __LINE__ << " mcp4725::writeDevice failed" << std::endl;
                goto out;
            }
            usleep(1000);
        }
    }

out:
    mcp4725.closeDevice();

    return 0;
}
