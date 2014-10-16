#include <iostream>

#include <unistd.h>

/// gpio headers
#include <gpio.h>

/// delays
static const int OnTime = 600;
static const int OffTime = 30;

/// pin to use
static const int iopin = GPIO::Gpio4;


int main(int argc, char **argv)
{
    Gpio gpio;

    /// open device (/dev/mem)
    if (gpio.openDevice() < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " openDevice failed" << std::endl;
        return 0;
    }

    /// set direction on the pin we use to output
    gpio.setGpioDirection(iopin, GPIO::GpioOutput);

    /// toggle outputs
    while (true) {
	/// switch on
	std::cout << "Timer on (gpio " << std::dec << iopin << ")" << std::endl;
	gpio.setGpio(iopin, GPIO::GpioOn);

        /// wait for MicroSeconds microseconds
	usleep(OnTime);

	/// switch off
	std::cout << "Timer off (gpio " << std::dec << iopin << ")" << std::endl;
	gpio.setGpio(iopin, GPIO::GpioOff);

	/// wait for MicroSeconds microseconds
	usleep(OffTime);
    }

    return 0;
}
