#include <iostream>

#include <unistd.h>
#include <string.h>

#include <pca9685.h>


PCA9685::PCA9685(char *device, int address, bool debug) :
    Firmware_I2C(device, address, debug),
    mMode1(0), mMode2(0), mOscillatorFrequency(DefaultOscFreq)
{

}

int PCA9685::setFrequency(unsigned int frequency)
{
    mPrescale = (int)((mOscillatorFrequency)/ (4096 * frequency));

    std::cout << "Frequency " << frequency << " Hz and oscillator " << mOscillatorFrequency / 1000000 << " MHz : ";
    std::cout << "Calculated prescale " << mPrescale << std::endl;

    mBuffer[0] = PCA9685Registers::Prescale;
    mBuffer[1] = mPrescale;

    int status = write(mFd, mBuffer, 2);
    if (status != 2) {
        std::cerr << "PCA9685::" << __func__ << "(): write failed with error " << status << std::endl;
        return -2;
    }

    return 0;
}

int PCA9685::writeOutput(unsigned char output, unsigned short ontime, unsigned short offtime)
{
    if (output > 15) {
        std::cerr << "PCA9685::" << __func__ << "(): output out off range [0 .. 15]" << std::endl;
        return -1;
    }

    if (ontime > 4095) {
        std::cerr << "PCA9685::" << __func__ << "(): ontime out of range [0 .. 4095]" << std::endl;
        return -2;
    }

    if (offtime > 4095) {
        std::cerr << "PCA9685::" << __func__ << "(): offtime out of range [0 .. 4095]" << std::endl;
        return -3;
    }

    unsigned int addressOnLo  = 0b00000110 + (output * 4) + 0;
    unsigned int addressOnHi  = 0b00000110 + (output * 4) + 1;
    unsigned int addressOffLo = 0b00000110 + (output * 4) + 2;
    unsigned int addressOffHi = 0b00000110 + (output * 4) + 3;

    unsigned char onTimeLo = ontime & 0xff;
    unsigned char onTimeHi = (ontime >> 8) & 0xff;
    unsigned char offTimeLo = offtime & 0xff;
    unsigned char offTimeHi = (offtime >> 8) & 0xff;

    mBuffer[0] = addressOnLo;
    mBuffer[1] = onTimeLo;
    int status = write(mFd, mBuffer, 2);
    if (status != 2) {
        std::cerr << "PCA9685::" << __func__ << "(): write failed with error " << status << std::endl;
        return -4;
    }

    mBuffer[0] = addressOnHi;
    mBuffer[1] = onTimeHi;
    status = write(mFd, mBuffer, 2);
    if (status != 2) {
        std::cerr << "PCA9685::" << __func__ << "(): write failed with error " << status << std::endl;
        return -5;
    }

    mBuffer[0] = addressOffLo;
    mBuffer[1] = offTimeLo;
    status = write(mFd, mBuffer, 2);
    if (status != 2) {
        std::cerr << "PCA9685::" << __func__ << "(): write failed with error " << status << std::endl;
        return -6;
    }

    mBuffer[0] = addressOffHi;
    mBuffer[1] = offTimeHi;
    status = write(mFd, mBuffer, 2);
    if (status != 2) {
        std::cerr << "PCA9685::" << __func__ << "(): write failed with error " << status << std::endl;
        return -7;
    }

    return 0;
}


int PCA9685::writeOutput(unsigned char output, int onoff, int hilo, unsigned char value)
{
    if (output < 0 || output > 15) {
        std::cerr << "PCA9685::" << __func__ << "(): requested output out off range (0 .. 15)" << std::endl;
        return -1;
    }

    unsigned int address = 0b00000110 + (output * 4) + onoff + hilo;

    mBuffer[0] = address;
    mBuffer[1] = value;

    int status = write(mFd, mBuffer, 2);
    if (status != 2) {
        std::cerr << "PCA9685::" << __func__ << "(): write failed with error " << status << std::endl;
        return -2;
    }

    return 0;
}


int PCA9685::setAddress(unsigned char address)
{
    if (address != PCA9685Addresses::PCA9685Address0 &&
            address != PCA9685Addresses::PCA9685Address1  &&
            address != PCA9685Addresses::PCA9685Address2  &&
            address != PCA9685Addresses::PCA9685Address3  &&
            address != PCA9685Addresses::PCA9685Address4  &&
            address != PCA9685Addresses::PCA9685Address5  &&
            address != PCA9685Addresses::PCA9685Address6  &&
            address != PCA9685Addresses::PCA9685Address7  &&
            address != PCA9685Addresses::PCA9685Address8  &&
            address != PCA9685Addresses::PCA9685Address9  &&
            address != PCA9685Addresses::PCA9685Address10 &&
            address != PCA9685Addresses::PCA9685Address11 &&
            address != PCA9685Addresses::PCA9685Address12 &&
            address != PCA9685Addresses::PCA9685Address13 &&
            address != PCA9685Addresses::PCA9685Address14 &&
            address != PCA9685Addresses::PCA9685Address15)
    {
        std::cout << "PCA9685::" << __func__ << "invalid address" << std::endl;
        return -1;
    }

    mAddress = address;

    return 0;
}
