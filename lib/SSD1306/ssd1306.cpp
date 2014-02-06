#include <iostream>

#include <unistd.h>

#include <ssd1306.h>
#include <binary.h>


SSD1306::SSD1306(char *device, unsigned char address) :
    Firmware_I2C(device, address) {}


int SSD1306::setAddress(unsigned char address)
{
    if (address != SSD1306Addresses::SSD1306Address0 && address != SSD1306Addresses::SSD1306Address1) {
        std::cerr << __func__ << "(): address given " << address << " is not a valid SSD1306 address" << std::endl;
        std::cerr << __func__ << "(): valid addresses are 0b01111000 or 0b01111010 " << std::endl;
        return -1;
    }

    if (mDebug) {
        std::cout << "SSD1306::" << __func__ << "(): ";
        Binary binary;
        binary.printByteAsBinary("Setting Address ", address);
    }

    mAddress = address;

    return 0;
}


int SSD1306::runCommand(unsigned char command)
{
#if 1
    mBuffer[0] = 0b00000000;
    mBuffer[1] = command;

    if (mDebug) {
        Binary binary;
        std::cerr << "SSD1306::" << __func__ << ":" << std::endl;
        binary.printByteAsBinary("byte 0 ", mBuffer[0]);
        binary.printByteAsBinary("byte 1 ", mBuffer[1]);
        std::cerr << std::endl;
    }

    //std::cout << "desciptor " << mFd << std::endl;

    int status = write(mFd, mBuffer, 2);
    if (status != 2) {
        std::cerr << "SSD1306::"  << __func__ << "(): write failed with status " << status << std::endl;
        return -1;
    }

#else
    mBuffer[0] = 0b00000000;

    if (mDebug) {
        Binary binary;
        std::cerr << "SSD1306::" << __func__ << ":" << std::endl;
        binary.printByteAsBinary("byte  ", mBuffer[0]);
        std::cerr << std::endl;
    }

    //std::cout << "desciptor " << mFd << std::endl;

    int status = write(mFd, mBuffer, 1);
    if (status != 1) {
        std::cerr << "SSD1306::"  << __func__ << "(): write failed with status " << status << std::endl;
        return -1;
    }

    mBuffer[0] = command;

    if (mDebug) {
        Binary binary;
        std::cerr << "SSD1306::" << __func__ << ":" << std::endl;
        binary.printByteAsBinary("byte  ", mBuffer[0]);
        std::cerr << std::endl;
    }

    status = write(mFd, mBuffer, 1);
    if (status != 1) {
        std::cerr << "SSD1306::"  << __func__ << "(): write failed with status " << status << std::endl;
        return -2;
    }
#endif
    return 0;
}


int SSD1306::writeData(char *data, int size)
{
    if (size > 1024) {
        std::cerr << "SSD1306::"  << __func__ << "(): size too big" << std::endl;
        return -1;
    }

    mBuffer[0] = ((mCo << 7) & 0x80) | ((mDc << 6) & 0xc0);

    if (mDebug) {
        Binary binary;
        std::cerr << "SSD1306::" << __func__ << ": ";
        binary.printByteAsBinary("First byte ", mBuffer[0]);
    }

    for (int i = 0; i < size; i++)
        mBuffer[i + 1] = data[i];

    int status = write(mFd, mBuffer, size + 1);
    if (status != size + 1) {
        std::cerr << "SSD1306::"  << __func__ << "(): write failed with status " << status << std::endl;
        return -2;
    }

    return 0;
}
