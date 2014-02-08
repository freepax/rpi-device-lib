#include <iostream>

#include <unistd.h>
#include <string.h>

#include <ssd1306.h>
#include <binary.h>


SSD1306::SSD1306(char *device, unsigned char address) :
    Firmware_I2C(device, address) {}


int SSD1306::setAddress(unsigned char address)
{
    if (address != SSD1306Addresses::SSD1306Address0 && address != SSD1306Addresses::SSD1306Address1) {
        std::cerr << __func__ << ":" << __LINE__ << " address given " << address << " is not a valid SSD1306 address" << std::endl;
        std::cerr << __func__ << ":" << __LINE__ << " valid addresses are 0b01111000 or 0b01111010 " << std::endl;
        return -1;
    }

    if (mDebug) {
        std::cout << "SSD1306::" << __func__ << ":" << __LINE__ << " hex address x0" << std::hex << address << std::dec << std::endl;
        Binary binary;
        binary.printByteAsBinary("bin address ", address);
    }

    mAddress = address;

    return 0;
}


int SSD1306::runCommand(unsigned char command)
{
    unsigned char buffer[] = { 0b00000000, command };

    if (mDebug) {
        Binary binary;
        std::cerr << "SSD1306::" << __func__ << ":" << __LINE__  << "hex buffer 0 and 1 0x" << std::hex << buffer[0] << " 0x" << buffer[1] << std::dec << std::endl;
        binary.printByteAsBinary("byte 0 ", buffer[0]);
        binary.printByteAsBinary("byte 1 ", buffer[1]);
        std::cerr << std::dec << std::endl;
    }

    int status = write(mFd, buffer, 2);
    if (status != 2) {
        std::cerr << "SSD1306::"  << __func__ << ":" << __LINE__ << " write failed with status " << status << std::endl;
        return -1;
    }

    return 0;
}


int SSD1306::writeRegister(unsigned char reg, unsigned char data)
{
    unsigned char buffer[] = { reg, data };

    int status = write(mFd, buffer, 2);
    if (status != 2) {
        std::cerr << "SSD1306::"  << __func__ << ":" << __LINE__ << " write failed with status " << status << std::endl;
        return -1;
    }

    return 0;
}


int SSD1306::writeByte(unsigned char byte)
{
    int status = write(mFd, &byte, 1);
    if (status != 1) {
        std::cerr << "SSD1306::"  << __func__ << ":" << __LINE__ << " write failed with status " << status << std::endl;
        return -1;
    }
    return 0;
}


int SSD1306::writeLine(unsigned char page, unsigned char data[128])
{
    if (runCommand(ssd1306ColumnAddress) < 0)
        return 0;
    if (runCommand(0) < 0)
        return 0;
    if (runCommand(127) < 0)
        return 0;

    if (runCommand(ssd1306PageAddress) < 0)
        return 0;
    if (runCommand(page) < 0)
        return 0;
    if (runCommand(page) < 0)
        return 0;

    unsigned char b[129];
    b[0] = 0x40;

    for (int i = 1; i < 128; i++)
        b[i] = data[i - 1];

    int status = write(mFd, b, 128);
    if (status < 0)
        return -1;

    return 0;
}

int SSD1306::writeChar(unsigned char line, unsigned char position, unsigned char character)
{
    position = position * 8;

    if (runCommand(ssd1306ColumnAddress) < 0)
        return 0;
    if (runCommand(position) < 0)
        return 0;
    if (runCommand(position) < 0)
        return 0;

    if (runCommand(ssd1306PageAddress) < 0)
        return 0;
    if (runCommand(line) < 0)
        return 0;
    if (runCommand(line) < 0)
        return 0;

    return 0;
}


int SSD1306::writeData()
{
    if (runCommand(ssd1306ColumnAddress) < 0)
        return 0;
    if (runCommand(0) < 0)
        return 0;
    if (runCommand(127) < 0)
        return 0;

    if (runCommand(ssd1306PageAddress) < 0)
        return 0;
    if (runCommand(0xb0) < 0)
        return 0;
    if (runCommand(0xb7) < 0)
        return 0;

    unsigned char data[17];
    memset(data, 0, 17);

    std::cout << "writing data" << std::endl;
    //for (int i = 0; i < 128; i += 16) {
    for (int i = 0; i < 1024; i += 16) {

        data[0] = 0x40;
#if 1
        for (int j = 1; j < 17; j++)
            data[j] = arduino[i + j - 1];
#else
        for (int j = 1; j < 17; j++) {
            int pos = i + 16 - j;
            std::cout << "pos " << pos << std::endl;
            data[j] = buffer[pos];
        }
#endif
        int status = write(mFd, data, 17);
        if (status < 0)
            return -3;
    }

    return 0;
}

int SSD1306::clear()
{
    /// DISPLAY
    if (runCommand(ssd1306ColumnAddress) < 0)
        return 0;
    if (runCommand(0) < 0)
        return 0;
    if (runCommand(127) < 0)
        return 0;

    if (runCommand(ssd1306PageAddress) < 0)
        return 0;
    if (runCommand(0) < 0)
        return 0;
    if (runCommand(7) < 0)
        return 0;

    unsigned char data[17] = { 0x40 };
    memset(data, 0, 17);
    data[0] = 0x40;

    for (int i = 0; i < 1024; i += 16) {
        int status = write(mFd, data, 17);
        if (status < 0)
            return -1;
    }

    return 0;
}
