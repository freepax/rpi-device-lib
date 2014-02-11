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

    int input = GPIO::Gpio4;

    /// poll input
    while (true) {
        /// wait for MicroSeconds microseconds
        usleep(MicroSeconds);
        gpio.setGpioDirection(input, GPIO::GpioInput);
        std::cout << "Gpio " << input << " status: " << gpio.getGpio(input) << std::endl;
    }

    return 0;
}
