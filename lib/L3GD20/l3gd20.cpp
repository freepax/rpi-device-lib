#include <iostream>

#include <unistd.h>

/// l3gd20 headers
#include <l3gd20.h>
#include <binary.h>


L3GD20::L3GD20(char *device, int address, bool debug) :
    Firmware_I2C(device, address, debug),
    mWho(L3dg20RegistersDefaults::mWhoDefaults),
    mControl_1(L3dg20RegistersDefaults::mControl_1Defaults), mControl_2(L3dg20RegistersDefaults::mControl_2Defaults),
    mControl_3(L3dg20RegistersDefaults::mControl_3Defaults), mControl_4(L3dg20RegistersDefaults::mControl_4Defaults),
    mControl_5(L3dg20RegistersDefaults::mControl_5Defaults), mReference(L3dg20RegistersDefaults::mReferenceDefaults),
    mFifoControl(L3dg20RegistersDefaults::mFifoControlDefaults),
    mInteruptConfigure(L3dg20RegistersDefaults::mInteruptConfigureDefaults),
    mInteruptXH(L3dg20RegistersDefaults::mInteruptXHDefaults),
    mInteruptXL(L3dg20RegistersDefaults::mInteruptXLDefaults), mInteruptYH(L3dg20RegistersDefaults::mInteruptYHDefaults),
    mInteruptYL(L3dg20RegistersDefaults::mInteruptYLDefaults), mInteruptzH(L3dg20RegistersDefaults::mInteruptzHDefaults),
    mInteruptzl(L3dg20RegistersDefaults::mInteruptzlDefaults), mInteruptDuration(L3dg20RegistersDefaults::mInteruptDurationDefaults)
{}


L3GD20::~L3GD20()
{
    closeDevice();
}


int L3GD20::whoAmI(unsigned char *serialnumber)
{
    Binary binary;
    binary.printByteAsBinary("Who am I register address", L3dg20RegistersAddresses::mWhoAddress);

    /// write the who am i address
    mBuffer[0] = (L3dg20RegistersAddresses::mWhoAddress);
    int status = write(mFd, mBuffer, 1);
    if (status < 0) {
        std::cerr << "L3dg20::" << __func__ << "(): write serial number address failed " << status << std::endl;
        return -1;
    }

    status = read(mFd, mBuffer, 1);
    if (status < 0) {
        std::cerr << "L3dg20::" << __func__ << "(): read serial number failed " << status << std::endl;
        return -2;
    }

    *serialnumber = mBuffer[0];

    return 0;
}

int L3GD20::readTemperatur(unsigned char *temperatur)
{
    Binary binary;
    binary.printByteAsBinary("I2C address", L3dg20RegistersAddresses::mOutZLAddress);

    mBuffer[0] = L3dg20RegistersAddresses::mOutZLAddress;
    int status = write(mFd, mBuffer, 1);
    if (status < 0) {
        std::cerr << "L3dg20::" << __func__ << "(): write temp out address failed " << status << std::endl;
        return -1;
    }

    status = read(mFd, mBuffer, 1);
    if (status < 0) {
        std::cerr << "L3dg20::" << __func__ << "(): read tempout data failed " << status << std::endl;
        return -2;
    }

    *temperatur = mBuffer[0];

    return 0;
}


int L3GD20::readZrotation(short *zrotation)
{
    /// set msb in address for reading multipple bytes (two bytes)
    mBuffer[0] = (L3dg20RegistersAddresses::mOutZLAddress & 0x80);
    int status = write(mFd, mBuffer, 1);
    if (status < 0) {
        std::cerr << "L3dg20::" << __func__ << "(): write zl address failed " << status << std::endl;
        return -1;
    }

    /// read two bytes
    status = read(mFd, mBuffer, 2);
    if (status != 2) {
        std::cerr << "L3dg20::" << __func__ << "(): read zl data failed " << status << std::endl;
        return -2;
    }

    *zrotation = (((mBuffer[1] & 0x0f) << 8) | (mBuffer[0] & 0x0f));

    return 0;
}


//int L3GD20::setControlOne(unsigned char dr, unsigned char bw, bool pd, bool zen, bool xen, bool yen)
int L3GD20::setControlOne()
{
    mBuffer[0] = L3dg20RegistersAddresses::mControl_1Address;
    mBuffer[1] = mControl_1;
    int status = write(mFd, mBuffer, 2);
    if (status < 0) {
        std::cerr << "L3GD20::" << __func__ << "(): write failed " << status << std::endl;
        return -1;
    }

    return 0;
}

int L3GD20::setControlTwo(unsigned char hpm, unsigned char hpc)
{

    return 0;
}

int L3GD20::setControlThree(bool int1_enable, bool int1_boot, bool lh_active, bool pp_open,
                    bool data_readu_2, bool fifo_wter_2, bool fifo_over_2, bool fifo_empt_2)
{

    return 0;
}


int L3GD20::setAddress(int addr)
{
    if (addr != L3dg20::L3gd20Address0 && addr != L3dg20::L3gd20Address1) {
        std::cerr << __func__ << "(): address given " << addr << " is not a valid L3GD20 address" << std::endl;
        std::cerr << __func__ << "(): valid addresses are 0b01101010 and 0b01101011 " << std::endl;
        return -1;
    }

    mAddress = addr;

    return 0;
}

