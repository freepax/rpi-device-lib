#include <iostream>

#include <unistd.h>

/// binary and mcp4725 headers
#include <binary.h>
#include <mcp4725.h>


MCP4725::MCP4725(char *device, unsigned char address) :
    Firmware_I2C(device, address),
    mControl(Mcp4725Config::ConfigFast1),
    mPowerDown(Mcp4725Power::PowerNormal) {}


int MCP4725::setAddress(unsigned char addr)
{
    if (addr != Mcp4725Addresses::Mcp4725Address0 && addr != Mcp4725Addresses::Mcp4725Address1) {
        std::cerr << __func__ << "(): address given " << addr << " is not a valid MCP4725 address" << std::endl;
        std::cerr << __func__ << "(): valid addresses are 0b01100010 and 0b01100011 " << std::endl;
        return -1;
    }

    if (mDebug) {
        std::cout << "MCP4725::" << __func__ << "(): ";
        Binary binary;
        binary.printByteAsBinary("Setting address ", addr);
    }

    mAddress = addr;

    return 0;
}


int MCP4725::setControl(unsigned char control)
{
    if (control > 7) {
        std::cerr << "MCP4725::" << __func__ << "(): value out of range" << std::endl;
        return -1;
    }

    mControl = control;

    return 0;
}


int MCP4725::setPowerDown(unsigned char power)
{
    if (power > 3) {
        std::cerr << "MCP4725::" << __func__ << "(): value out of range" << std::endl;
        return -1;
    }

    mPowerDown = power;

    return 0;
}


int MCP4725::writeDevice(unsigned short value)
{
    int bytes = 0;

    switch (mControl) {
    case 0:
    case 1:
        //std::cout << "case 0, 1" << std::endl;
        /// [C2 C1 PD1 PD0 D11 D10 D9 D8]
        mBuffer[0] = ((mControl & 0x06) << 6) | ((mPowerDown & 0x03) << 4) | ((value & 0xFF00) >> 8) & 0x0F;

        /// [D7 D6 D5  D4  D3  D2  D1 D0]
        mBuffer[1] = (value & 0x00FF);
        bytes = 2;
        break;
    case 2:
    case 3:
        //std::cout << "case 2, 3" << std::endl;
        /// set the 12 bits on dac, c2 and c1 = 0 and pd1 and pd0 = 0
        mBuffer[0] = ((mControl & 0x07) << 5) | ((mPowerDown & 0x03) << 1); /// [C2 C1 C0 X X PD1 PD0 X ]
        mBuffer[1] = ((value & 0x00000ff0) >> 4);                           /// [D11 D10 D9 D8 D7 D6  D5  D4]
        mBuffer[2] = (value & 0x0000000f) << 4;                             /// [D3  D2  D1 D0  X  X   X   X]
        bytes = 3;
        break;
    }

    if (mDebug) {
        Binary binary;
        std::cout << "MCP4725::" << __func__  << "(): ";
        binary.printByteAsBinary("Buffer 0 ", mBuffer[0]);
        std::cout << "MCP4725::" << __func__  << "(): ";
        binary.printByteAsBinary("Buffer 1 ", mBuffer[1]);
        if (bytes == 3) {
            std::cout << "MCP4725::" << __func__  << "(): ";
            binary.printByteAsBinary("Buffer 2 ", mBuffer[2]);
        }
    }

    /// write device
    int status = write(mFd, mBuffer, bytes);
    if (status != bytes) {
        std::cerr << "MCP4725::" << __func__ << "(): write failed with error " << status << std::endl;
        return -1;
    }

    return 0;
}


int MCP4725::readDevice(unsigned char *config, unsigned short *dac, unsigned char bytes)
{
    if (bytes < 3)
        bytes = 3;

    /// read the i2c device
    int status = read(mFd, mBuffer, bytes);
    if (status != bytes) {
        std::cerr << "MCP4725::" << __func__ << "(): read failed with error " << status << std::endl;
        return -1;
    }

    *config = mBuffer[0];
    *dac = (mBuffer[1] << 4) | (mBuffer[2] >> 4);

    if (mDebug) {
        Binary binary;
        for (int i = 0; i < bytes; i++) {
            //std::cout << i << " ";
            binary.printByteAsBinary(" ", mBuffer[i]);
        }
    }

    return 0;

}


void MCP4725::resolveConfig(unsigned char config)
{
    if (config & Busy)
        std::cout << "  * EPROM WRITE READY" << std::endl;
    else
        std::cout << "  * EPROM WRITE BUSY" << std::endl;

    if (config && Por)
        std::cout << "  * POR on" << std::endl;
    else
        std::cout << "  * POR off" << std::endl;

    std::cout << "  * PD1 PD0 ";
    if (config & Pd1)
        std::cout << "1 ";
    else
        std::cout << "0 ";

    if (config & Pd0)
        std::cout << "1 ";
    else
        std::cout << "0 ";
    std::cout << std::endl;

    return;
}
