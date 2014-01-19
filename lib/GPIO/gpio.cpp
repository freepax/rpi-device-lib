#include <iostream>

#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#include <gpio.h>


Gpio::Gpio() : mFd(-1), mGpio(NULL), mDebug(false) {}


Gpio::~Gpio()
{
    closeDevice();
}


int Gpio::openDevice()
{
    /// close device first if open
    if (mFd != -1) {
        close(mFd);
        mFd = -1;
    }

    /// open /dev/mem
    std::cout << "Opening device" << DevMem << std::endl;
    mFd = open(DevMem, O_RDWR|O_SYNC);
    if (mFd < 0) {
        std::cerr << __func__ << "(): open " << DevMem << " failed" << std::endl;
        return -1;
    }

    if (mGpio != NULL) {
        std::cerr << __func__ << "(): gpio memory already mapped" << std::endl;
        return -2;
    }

    /// mmap GPIO
    std::cout << "Mapping " << DevMem << std::endl;
    mGpio = (unsigned*)mmap(NULL, BlockSize, PROT_READ|PROT_WRITE, MAP_SHARED, mFd, GpioBase);
    if (mGpio == MAP_FAILED) {
        std::cerr << __func__ << "(): mmap failed" << std::endl;
        return -2;
    }

    if (close(mFd) < 0) {
        std::cerr << __func__ << "(): close failed" << std::endl;
        mFd = -1;
        return -1;
    }

    mFd = -1;

    return 0;
}

int Gpio::closeDevice()
{
    /// munmap GPIO
    std::cout << "Checking file descriptor" << std::endl;
    if (mFd != -1) {
        close(mFd);
        mFd = -1;
    }

    std::cout << "Munmap " << DevMem << std::endl;
    if (munmap(mGpio, BlockSize) < 0) {
        std::cerr << __func__ << "(): munmap failed" << std::endl;
        return -1;
    }

    mGpio = NULL;
}


/// Set gpio pin to input or output
void Gpio::setGpioDirection(int pin, int direction)
{
    //std::cout << __func__ << "pin "<< pin << " direction " << direction << std::endl;
    //std::cout << "pin "<< pin << *(mGpio + (pin / 10)) << std::endl;

    switch (direction) {
    case GPIO::GpioInput:  *(mGpio + (pin / 10)) &= ~(7 << ((pin % 10) * 3)); break;
    case GPIO::GpioOutput: *(mGpio + (pin / 10)) |=  (1 << ((pin % 10) * 3)); break;
    default: std::cerr << __func__ << "(): Unknown direction " << direction << std::endl;
    }
}

/// Set gpio pin to (binary) value
void Gpio::setGpio(int pin, int value)
{
    //std::cout << __func__ << "value " << value << std::endl;

    switch (value) {
    case GPIO::GpioOff: *(mGpio + 10) = 1 << pin; break;
    case GPIO::GpioOn:  *(mGpio + 7) = 1 << pin; break;
    default: std::cerr << __func__ << "(): iligal value " << value << std::endl;
    }
}
