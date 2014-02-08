#include <iostream>

#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

/// firmware_i2c and binary headers
#include <firmware_i2c.h>
#include <binary.h>


Firmware_I2C::Firmware_I2C(char *device, unsigned char address, bool debug) : mFd(-1), mDevice(device), mAddress(address), mDebug(debug)
{
    if (mDebug) {
        std::cout << __func__ << ":" << __LINE__ << " device: "<< mDevice << " ";
        Binary binary;
        binary.printByteAsBinary("Address ", address);
    }
}


Firmware_I2C::~Firmware_I2C()
{
    if (mDebug)
        std::cout << "Firmware_I2C::" << __func__ << ":" << __LINE__ << " closing device" << std::endl;

    closeDevice();
}


int Firmware_I2C::openDevice()
{
    /// close if open
    if (mFd > 0)
        closeDevice();

    /// Open I2C device
    if (mDebug)
        std::cout << "Firmware_I2C::" << __func__ << ":" << __LINE__ << " opening device " << mDevice << std::endl;

    mFd = open(mDevice, O_RDWR);
    if (mFd < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " openI2C failed with error " << mFd << std::endl;
        return -1;
    }

    if (mDebug) {
        std::cerr <<  "Firmware_I2C::" << __func__ << ":" << __LINE__ << " hex address " << std::hex << mAddress << std::dec << std::endl;
        Binary binary;
        binary.printByteAsBinary("bin address", mAddress);
        std::cerr << std::endl;
    }

    /// Set i2c device address in i2c driver for this session
    int status = ioctl(mFd, I2C_SLAVE, mAddress);
    if (status < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " ioctl failed with error " << status << std::endl;
        return -2;
    }

    return 0;
}


int Firmware_I2C::writeData(unsigned char *data, int size)
{
    if (mDebug) {
        Binary binary;
        for (int i = 0; i < size; i++)
            binary.printByteAsBinary("Data ", mBuffer[i]);
    }

    int status = write(mFd, data, size);
    if (status < 0) {
        std::cerr << "Firmware_I2C::" << __func__ << ":" << __LINE__ << " write failed " << status << std::endl;
        return -1;
    }

    return 0;
}


int Firmware_I2C::readData(unsigned char *data, int size)
{
    int status = read(mFd, data, size);
    if (status != size) {
        std::cerr << "Firmware_I2C::" << __func__ << ":" << __LINE__ << " read failed " << status << std::endl;
        return -2;
    }

    return 0;
}


int Firmware_I2C::closeDevice()
{
    if (mFd != -1) {
        int status = close(mFd);
        mFd = -1;
        return status;
    }

    return 0;
}


int Firmware_I2C::setDevice(char *device)
{
    /// check that device is valid
    if (strcmp(device, FirmwareI2CDeviceses::i2c_0)     != 0 && strcmp(device, FirmwareI2CDeviceses::i2c_1) != 0 &&
            strcmp(device, FirmwareI2CDeviceses::i2c_2) != 0 && strcmp(device, FirmwareI2CDeviceses::i2c_3) != 0 &&
            strcmp(device, FirmwareI2CDeviceses::i2c_4) != 0 && strcmp(device, FirmwareI2CDeviceses::i2c_5) != 0 &&
            strcmp(device, FirmwareI2CDeviceses::i2c_6) != 0 && strcmp(device, FirmwareI2CDeviceses::i2c_7) != 0) {
        std::cerr << "Firmware_I2C::" << __func__ << ":" << __LINE__ << " device " << device << " not a valid device" << std::endl;
        return -1;
    }

    /// set device
    mDevice = device;

    return 0;
}
