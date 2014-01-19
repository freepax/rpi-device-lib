#include <iostream>

#include <unistd.h>

/// binary and mcp4725 headers
#include <binary.h>
#include <mcp4725.h>


MCP4725::MCP4725(char *device, unsigned char address, bool debug) :
    Firmware_I2C(device, address, debug), mConfig(-1)
{
    if (mDebug) {
        std::cout << "MCP4725::" <<  __func__ << "(): device: " << mDevice << std::endl;
        std::cout << "MCP4725::" << __func__ << "(): addres: " << std::endl;
        Binary binary;
        binary.printByteAsBinary(mAddress);
    }
}


MCP4725::~MCP4725()
{
    closeDevice();
}


int MCP4725::setAddress(unsigned char addr)
{
    if (addr != Mcp4725Addresses::Mcp4725Address0 && addr != Mcp4725Addresses::Mcp4725Address1) {
        std::cerr << __func__ << "(): address given " << addr << " is not a valid MCP4725 address" << std::endl;
        std::cerr << __func__ << "(): valid addresses are 0b01100010 and 0b01100011 " << std::endl;
        return -1;
    }

    if (mDebug) {
        std::cout << "MCP4725::" << __func__ << "(): setting address " << std::endl;
        Binary binary;
        binary.printByteAsBinary(addr);
    }

    mAddress = addr;

    return 0;
}

int MCP4725::writeDevice(float voltage)
{
    return writeDevice((int)(voltage / 5.0 * 4095));
}


int MCP4725::writeDevice(int value)
{
    /// set the 12 bits on dac, c2 and c1 = 0 and pd1 and pd0 = 0
    mBuffer[0] = ((value & 0xFF00) >> 8) & 0x0F;        /// [C2 C1 PD1 PD0 D11 D10 D9 D8]
    mBuffer[1] = (value & 0x00FF);                      /// [D7 D6 D5  D4  D3  D2  D1 D0]
    //printf("mBuffer [0]: 0x%02x 0x%02x\n", mBuffer[0], mBuffer[1]);

    int status = write(mFd, mBuffer, 2);
    if (status != 2) {
        std::cerr << "MCP4725::" << __func__ << "(): write failed with error " << status << std::endl;
        return -1;
    }

    return 0;
}


int MCP4725::writeDeviceEeprom(float voltage)
{
    return writeDeviceEeprom((int)(voltage / 5.0 * 4095));
}


int MCP4725::writeDeviceEeprom(int value)
{
    /// C2 = 0 C1 = 1 C0 = 1 (write eeprom) PD1 = 0 and PD0 = 0
    mBuffer[0] = 0b01100000;          /// [C2  C1  C0  X  X PD1 PD0  X]
    mBuffer[1] = ((value & 0x00000ff0) >> 4);  /// [D11 D10 D9 D8 D7 D6  D5  D4]
    mBuffer[2] = (value & 0x0000000F) << 4;    /// [D3  D2  D1 D0  X  X   X   X]
    //printf("0x%02x 0x%02x 0x%02x\n", mBuffer[0], mBuffer[1], mBuffer[2]);

    /// write the i2c device
    int status = write(mFd, mBuffer, 3);
    if (status != 3) {
        std::cerr << "MCP4725::" << __func__ << "(): write failed with error " << status << std::endl;
        return -1;
    }

    return 0;
}

int MCP4725::readDevice()
{
    /// read the i2c device
    int status = read(mFd, mBuffer, 3);
    if (status != 3) {
        std::cerr << "MCP4725::" << __func__ << "(): read failed with error " << status << std::endl;
        return -1;
    }

    mConfig = (mBuffer[0] << 16) | (mBuffer[1] << 8) | mBuffer[2];

    if (mDebug) std::cout << "MCP4725::" << __func__ << "(): " << mConfig << std::endl;

    return 0;
}

void MCP4725::status(void)
{
    if (mConfig == -1) {
        std::cerr << __func__ << ": config not set" << std::endl;
        return;
    }

    if (mConfig & 0x00008000)
        //if (mBuffer[0] & 0x80)
        std::cout << "  * EPROM WRITE READY" << std::endl;
    else
        std::cout << "  * EPROM WRITE BUSY" << std::endl;

    if (mConfig && 0x00004000)
        //if (mBuffer[0] & 0x40)
        std::cout << "  * POR on" << std::endl;
    else
        std::cout << "  * POR off" << std::endl;

    if (mConfig & 0x00000400)
        //if (mBuffer[0] & 0x04)
        std::cout << "  * PD1 is hi" << std::endl;
    else
        std::cout << "  * PD1 is lo" << std::endl;

    if (mConfig & 0x00000200)
        //if (mBuffer[0] & 0x02)
        std::cout << "  * PD0 is hi" << std::endl;
    else
        std::cout << "  * PD0 is lo" << std::endl;

    mConfig = -1;
}
