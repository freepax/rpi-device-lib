#include <iostream>

#include <unistd.h>

/// gpio headers
#include <gpio.h>


static const int MicroSeconds = 200000;


int main(int argc, char **argv)
{
    Gpio gpio;

    /// open device (/dev/mem)
    if (gpio.openDevice() < 0) {
        std::cerr << __func__ << "(): openDevice failed" << std::endl;
        return 0;
    }

    /// set all gpio's as output - skip Gpio2 - this is i2c SCL
    for (int i = 0; i < GPIO::GpioSize; i++) {
        if (GPIO::GpioList[i] == GPIO::Gpio2) continue;
        gpio.setGpioDirection(GPIO::GpioList[i], GPIO::GpioOutput);
    }

    /// toggle outputs
    for (int loop = 0; loop < 5; loop++) {
        /// wait for MicroSeconds microseconds
        usleep(MicroSeconds);

        /// switch all gpio's on - skip Gpio2 - this is i2c SCL
        std::cout << "Turning all gpio's on" << std::endl;
        for (int i = 0; i < GPIO::GpioSize; i++) {
            if (GPIO::GpioList[i] == GPIO::Gpio2) continue;
            gpio.setGpio(GPIO::GpioList[i], GPIO::GpioOn);
        }

        /// wait for MicroSeconds microseconds
        usleep(MicroSeconds);

        /// switch all gpio's off - skip Gpio2 - this is i2c SCL
        std::cout << "Turning all gpio's off" << std::endl;
        for (int i = 0; i < GPIO::GpioSize; i++) {
            if (GPIO::GpioList[i] == GPIO::Gpio2) continue;
            gpio.setGpio(GPIO::GpioList[i], GPIO::GpioOff);
        }
    }

    return 0;
}
