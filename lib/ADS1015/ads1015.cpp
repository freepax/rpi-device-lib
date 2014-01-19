#include <iostream>

#include <unistd.h>

/// ads1015 headers
#include <ads1015.h>


ADS1015::ADS1015(char *device, int address, bool debug) :
    Firmware_I2C(device, address, debug),
    mOs(OSNoEffectOnWrite), mMux(MUXAIN0GND), mPga(PGA2048), mMode(MODEContinous), mVref(2.048),
    mDr(DR128), mCompMode(COMPMODEHysteresis), mCompPol(COMPPOLActiveLow), mCompLat(COMPLATNoneLatching), mCompQue(COMPQUEDisable) {}


ADS1015::~ADS1015()
{
    if (mFd > 0)
        closeDevice();
}


int ADS1015::setOs(unsigned int value)
{
    /// OS is one bit
    if (value < 0 || value > 1)
        return -1;

    mOs = value;

    return 0;
}


int ADS1015::setMux(unsigned int value)
{
    /// mux is three bits (0 through 7 inclusive)
    if (value < 0 || value > 7)
        return -1;

    /// set mux to the given value
    mMux = value;

    return 0;
}


int ADS1015::setPga(unsigned int value)
{
    /// pga is three bits (0 through 7 inclusive)
    if (value < 0 || value > 7)
        return -1;

    /// just set pga to the given value
    mPga = value;

    /// adjust mVref according to pga
    switch (value) {
    case PGA6144:   mVref = 6.144; break;
    case PGA4096:   mVref = 4.096; break;
    case PGA2048:   mVref = 2.048; break;
    case PGA1024:   mVref = 1.024; break;
    case PGA0512:   mVref = 0.512; break;
    case PGA0256_1: mVref = 0.256; break;
    case PGA0256_2: mVref = 0.256; break;
    case PGA0512_3: mVref = 0.256; break;
    default: return -2; /// should not happend since we passed the range test above
    }

    return 0;
}


int ADS1015::setMode(unsigned int value)
{
    if (value < 0 || value > 1)
        return -1;

    /// accept mode
    mMode = value;

    return 0;
}


int ADS1015::setDr(unsigned int value)
{
    /// dr is three bits
    if (value < 0 || value > 7)
        return -1;

    mDr = value;

    return 0;
}


int ADS1015::setCompMode(unsigned int value)
{
    /// mCompMode is one bit
    if (value < 0 || value > 1)
        return -1;

    mCompMode = value;

    return 0;
}


int ADS1015::setCompPol(unsigned int value)
{
    /// mCompPol is one bit
    if (value < 0 || value > 1)
        return -1;

    mCompPol = value;

    return 0;
}


int ADS1015::setCompLat(unsigned int value)
{
    /// mCompLat is one bit
    if (value < 0 || value > 1)
        return -1;

    mCompLat = value;

    return 0;
}


int ADS1015::setCompQue(unsigned int value)
{
    /// mCompQue is two bit
    if (value < 0 || value > 3)
        return -1;

    mCompQue = value;

    return 0;
}


int ADS1015::writeConfig()
{
    mBuffer[0] = PointerConfigRegister;
    mBuffer[1] = mOs << 7 | mMux << 4 | mPga << 1 | mMode;
    mBuffer[2] = mDr << 5 | mCompMode << 4 | mCompPol << 3 | mCompLat << 2 | mCompQue;

    int status = write(mFd, mBuffer, 3);
    if (status != 3) {
        std::cerr << __func__ << ": write failed with error " << status << std::endl;
        return -1;
    }

    return 0;
}


int ADS1015::initRead()
{
    mBuffer[0] = PointerConversionRegister;

    int status = write(mFd, mBuffer, 1);
    if (status != 1) {
        std::cerr << __func__ << ": write failed with error" <<  status;
        return -1;
    }

    return 0;
}


float ADS1015::readDevice()
{
    int status = read(mFd, mBuffer, 2);
    if (status != 2) {
        std::cerr << __func__ << ": read returned" << status;
        return ConversionOutOfRange;
    }

    return (mVoltage = mVref / 4096 * ((mBuffer[0] << 4) | (mBuffer[1] >> 4)));
}
