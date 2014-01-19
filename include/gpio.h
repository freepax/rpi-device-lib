#ifndef LIB_GPIO_H
#define LIB_GPIO_H


static const int Bcm2708PeriferalBase = 0x20000000;
static const int GpioBase = 0x20000000 + 0x200000;
static const int BlockSize = (4 * 1024);
static const char DevMem[] = "/dev/mem";


namespace GPIO {
/// gpio as input or output
static const int GpioInput = 0;
static const int GpioOutput = 1;

/// hi or lo output on pin
static const int GpioOff    = 0;
static const int GpioOn     = 1;


/// all the gpio pins on model B rev 2
static const int Gpio2  = 3;
static const int Gpio3  = 5;
static const int Gpio4  = 7;

static const int Gpio7  = 26;
static const int Gpio8  = 24;
static const int Gpio9  = 21;
static const int Gpio10 = 19;
static const int Gpio11 = 23;

static const int Gpio14 = 8;
static const int Gpio15 = 10;

static const int Gpio17 = 11;
static const int Gpio18 = 12;

static const int Gpio22 = 15;
static const int Gpio23 = 16;
static const int Gpio24 = 18;
static const int Gpio25 = 22;

static const int Gpio27 = 13;


/// list of gpio pins (GPIO2 and GPIO3 are SDA and SCL)
static const int GpioList[] = {
    Gpio2,  Gpio3,  Gpio4,
    Gpio7,  Gpio8,  Gpio9,  Gpio10, Gpio11,
    Gpio14, Gpio15,
    Gpio17, Gpio18,
    Gpio22, Gpio23, Gpio24, Gpio25,
    Gpio27
};

/// size of gpio list
static const int GpioSize = 17;
}


/// class gpio
class Gpio {
public:
    Gpio();
    ~Gpio();

    /// open device actually map gpio memory (using mmap)
    int openDevice();
    int closeDevice();

    /// set the direction
    void setGpioDirection(int pin, int direction);

    /// set value on gpio
    void setGpio(int pin, int value);

private:
    int mFd;
    unsigned *mGpio;
    bool mDebug;
};

#endif // LIB_GPIO_H
