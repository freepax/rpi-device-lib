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
        std::cerr << __func__ << ":" << __LINE__ << " openDevice failed" << std::endl;
        return 0;
    }

#if 0
    /// set all gpio's as output - skip Gpio2 (i2c SCL) and Gpio3 (i2c data)
    for (int i = 0; i < GPIO::GpioSize; i++) {
        if (GPIO::GpioList[i] == GPIO::Gpio2) continue;                 /// i2c scl
        if (GPIO::GpioList[i] == GPIO::Gpio3) continue;                 /// i2c data
        gpio.setGpioDirection(GPIO::GpioList[i], GPIO::GpioOutput);
    }
#endif

    int output = GPIO::Gpio4;

    gpio.setGpioDirection(output, GPIO::GpioOutput);


    /// toggle outputs
    //for (int loop = 0; loop < 100; loop++) {
    while (true) {
        /// wait for MicroSeconds microseconds
        usleep(MicroSeconds);

        /// switch all gpio's on - skip Gpio2 and Gpio3- this is i2c SCL and sda
#if 0
        std::cout << "Turning all gpio's on" << std::endl;
        for (int i = 0; i < GPIO::GpioSize; i++) {
            if (GPIO::GpioList[i] == GPIO::Gpio2) continue;
            if (GPIO::GpioList[i] == GPIO::Gpio3) continue;
            gpio.setGpio(GPIO::GpioList[i], GPIO::GpioOn);
        }
#endif
        std::cout << "Turning on gpio " << std::dec << output << std::endl;
        gpio.setGpio(output, GPIO::GpioOn);

        /// wait for MicroSeconds microseconds
        usleep(MicroSeconds);

        std::cout << "Turning off gpio " << std::dec << output << std::endl;
        gpio.setGpio(output, GPIO::GpioOff);

#if 0
        /// switch all gpio's off - skip Gpio2 - this is i2c SCL
        std::cout << "Turning all gpio's off" << std::endl;
        for (int i = 0; i < GPIO::GpioSize; i++) {
            if (GPIO::GpioList[i] == GPIO::Gpio2) continue;
            if (GPIO::GpioList[i] == GPIO::Gpio3) continue;
            gpio.setGpio(GPIO::GpioList[i], GPIO::GpioOff);
        }
#endif
    }

    return 0;
}
