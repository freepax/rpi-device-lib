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


Firmware_I2C::Firmware_I2C(char *device,unsigned char address, bool debug) : mFd(-1), mDevice(device), mAddress(address), mDebug(debug)
{
    if (mDebug) {
        std::cout << __func__ << "(): device: "<< mDevice << " addres: " << std::endl;
        Binary binary;
        binary.printByteAsBinary(address);
    }
}


Firmware_I2C::~Firmware_I2C()
{
    if (mDebug)
        std::cout << "Firmware_I2C::" << __func__ << "(): closing device" << std::endl;

    closeDevice();
}


int Firmware_I2C::openDevice()
{
    /// close if open
    if (mFd > 0)
        closeDevice();

    /// Open I2C device
    if (mDebug)
        std::cout << "Firmware_I2C::" << __func__ << "(): opening device " << mDevice << std::endl;

    mFd = open(mDevice, O_RDWR);
    if (mFd < 0) {
        std::cerr << __func__ << "(): openI2C failed with error " << mFd << std::endl;
        return -1;
    }

    if (mDebug) {
        std::cout <<  "Firmware_I2C::" << __func__ << "(): setting address ";
        Binary binary;
        binary.printByteAsBinary(mAddress);
    }

    /// Set i2c bus address on i2c device
    int status = ioctl(mFd, I2C_SLAVE, mAddress);
    if (status < 0) {
        std::cerr << __func__ << "(): ioctl failed with error " << status << std::endl;
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
    if (strcmp(device, FirmwareI2CDeviceses::i2c_0) != 0 && strcmp(device, FirmwareI2CDeviceses::i2c_1) != 0 &&
            strcmp(device, FirmwareI2CDeviceses::i2c_2) != 0 && strcmp(device, FirmwareI2CDeviceses::i2c_3) != 0 &&
            strcmp(device, FirmwareI2CDeviceses::i2c_4) != 0 && strcmp(device, FirmwareI2CDeviceses::i2c_5) != 0 &&
            strcmp(device, FirmwareI2CDeviceses::i2c_6) != 0 && strcmp(device, FirmwareI2CDeviceses::i2c_7) != 0) {
        std::cerr << "Firmware_I2C::" << __func__ << "(): device " << device << " not a valid device" << std::endl;
        return -1;
    }

    /// set device
    mDevice = device;

    return 0;
}
