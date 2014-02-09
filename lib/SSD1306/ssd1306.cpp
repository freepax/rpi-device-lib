#include <iostream>
#include <iomanip>

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


int SSD1306::writeLine(unsigned char page, unsigned char data[Ssd1306LcdWitdh])
{
    /// set column
    if (runCommand(ssd1306ColumnAddress) < 0) {
        std::cerr << "SSD1306::"  << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -1;
    }

    if (runCommand(0) < 0) {
        std::cerr << "SSD1306::"  << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -2;
    }

    if (runCommand(Ssd1306LcdWitdh - 1) < 0) {
        std::cerr << "SSD1306::"  << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -3;
    }

    /// set page - set from/to the same
    if (runCommand(ssd1306PageAddress) < 0) {
        std::cerr << "SSD1306::"  << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -4;
    }

    if (runCommand(page) < 0) {
        std::cerr << "SSD1306::"  << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -5;
    }

    if (runCommand(page) < 0) {
        std::cerr << "SSD1306::"  << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -6;
    }

    unsigned char buffer[Ssd1306LcdWitdh + 1] = { 0x40 };

    for (int i = 1; i < Ssd1306LcdWitdh; i++)
        buffer[i] = data[i - 1];

    if (write(mFd, buffer, Ssd1306LcdWitdh + 1) < 0) {
        std::cerr << "SSD1306::"  << __func__ << ":" << __LINE__ << " write failed" << std::endl;
        return -7;
    }

    return 0;
}


int SSD1306::writeByte(unsigned char line, unsigned char position, unsigned char data)
{
    position = position * 8;

    /// set column - set from/to the same
    if (runCommand(ssd1306ColumnAddress) < 0) {
        std::cerr << "SSD1306::"  << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -1;
    }

    if (runCommand(position) < 0) {
        std::cerr << "SSD1306::"  << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -2;
    }

    if (runCommand(position+8) < 0) {
        std::cerr << "SSD1306::"  << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -3;
    }

    /// set page - set from/to the same
    if (runCommand(ssd1306PageAddress) < 0) {
        std::cerr << "SSD1306::"  << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -4;
    }

    if (runCommand(line) < 0) {
        std::cerr << "SSD1306::"  << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -5;
    }

    if (runCommand(line) < 0) {
        std::cerr << "SSD1306::"  << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -6;
    }

    unsigned char buffer[] = { 0x40, data, data, data, data, data, data, data, data };

    if (write(mFd, buffer, 9) < 0) {
        std::cerr << "SSD1306::"  << __func__ << ":" << __LINE__ << " write failed" << std::endl;
        return -7;
    }

    return 0;
}


int SSD1306::clear()
{
    /// set column
    if (runCommand(ssd1306ColumnAddress) < 0) {
        std::cerr << "SSD1306::"  << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -1;
    }

    if (runCommand(0) < 0) {
        std::cerr << "SSD1306::"  << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -2;
    }

    if (runCommand(Ssd1306LcdWitdh - 1) < 0) {
        std::cerr << "SSD1306::"  << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -3;
    }

    /// set page
    if (runCommand(ssd1306PageAddress) < 0) {
        std::cerr << "SSD1306::"  << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -4;
    }

    if (runCommand(0) < 0) {
        std::cerr << "SSD1306::"  << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -5;
    }

    if (runCommand(SSD1306LcdPages - 1) < 0) {
        std::cerr << "SSD1306::"  << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -6;
    }

    unsigned char data[17] = { 0x40 };
    memset(data, 0, 17);
    data[0] = 0x40;

    for (int i = 0; i < 1024; i += 16) {
        if (write(mFd, data, 17) < 0) {
            std::cerr << "SSD1306::"  << __func__ << ":" << __LINE__ << " write failed" << std::endl;
            return -7;
        }
    }

    return 0;
}
