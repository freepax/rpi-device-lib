#include <iostream>

#include <unistd.h>

/// l3gd20 headers
#include <l3gd20.h>
#include <binary.h>


L3GD20::L3GD20(char *device, int address, bool debug) :
    Firmware_I2C(device, address, debug),
    mWho(L3dg20Defaults::Who),
    mControl_1(L3dg20Defaults::ControlOne), mControl_2(L3dg20Defaults::ControlTwo),
    mControl_3(L3dg20Defaults::ControlThree), mControl_4(L3dg20Defaults::ControlFour),
    mControl_5(L3dg20Defaults::ControlFive), mReference(L3dg20Defaults::Reference),
    mFifoControl(L3dg20Defaults::FifoControl),
    mInteruptConfigure(L3dg20Defaults::InteruptConfigure),
    mInteruptXH(L3dg20Defaults::InteruptXH),
    mInteruptXL(L3dg20Defaults::InteruptXL), mInteruptYH(L3dg20Defaults::InteruptYH),
    mInteruptYL(L3dg20Defaults::InteruptYL), mInteruptzH(L3dg20Defaults::InteruptzH),
    mInteruptzl(L3dg20Defaults::Interuptzl), mInteruptDuration(L3dg20Defaults::InteruptDuration)
{}


L3GD20::~L3GD20()
{
    closeDevice();
}


int L3GD20::whoAmI(unsigned char *serialnumber)
{
    unsigned char buffer[] = { L3dg20Register::WhoAddress };

    /// write the who am i address
    int status = write(mFd, buffer, 1);
    if (status != 1) {
        std::cerr << "L3dg20::" << __func__ << ":" << __LINE__ << " write serial number address failed " << status << std::endl;
        return -1;
    }

    status = read(mFd, buffer, 1);
    if (status != 1) {
        std::cerr << "L3dg20::" << __func__ << ":" << __LINE__ << " read serial number failed " << status << std::endl;
        return -2;
    }

    *serialnumber = buffer[0];

    return 0;
}


int L3GD20::readTemperatur(unsigned char *temperatur)
{
    unsigned char buffer[] = { L3dg20Register::OutZLAddress };

    int status = write(mFd, buffer, 1);
    if (status != 1) {
        std::cerr << "L3dg20::" << __func__ << ":" << __LINE__ << " write temp out address failed " << status << std::endl;
        return -1;
    }

    status = read(mFd, buffer, 1);
    if (status != 1) {
        std::cerr << "L3dg20::" << __func__ << ":" << __LINE__ << " read tempout data failed " << status << std::endl;
        return -2;
    }

    *temperatur = buffer[0];

    return 0;
}


int L3GD20::readZrotation(short *zrotation)
{
    /// set msb in address for reading multipple bytes (two bytes)
    unsigned char buffer[2] = { (L3dg20Register::OutZLAddress & 0x80) };
    //mBuffer[0] = (L3dg20Register::mOutZLAddress & 0x80);
    int status = write(mFd, buffer, 1);
    if (status != 1) {
        std::cerr << "L3dg20::" << __func__ << ":" << __LINE__ << " write zl address failed " << status << std::endl;
        return -1;
    }

    /// read two bytes
    status = read(mFd, buffer, 2);
    if (status != 2) {
        std::cerr << "L3dg20::" << __func__ << ":" << __LINE__ << " read zl data failed " << status << std::endl;
        return -2;
    }

    *zrotation = (((buffer[1] & 0x0f) << 8) | (buffer[0] & 0x0f));

    return 0;
}

int L3GD20::writeRegister(unsigned char address, unsigned char byte)
{
    if (address == L3dg20Register::ControlFourAddress)
        byte &= 0b11110001;     /// bit three is "don't care", but don't touch bit 1 and 2

    unsigned char buffer[] = { address, byte };
    int status = write(mFd, buffer, 2);
    if (status != 2) {
        std::cerr << "L3GD20::" << __func__ << ":" << __LINE__ << " write failed " << status << std::endl;
        return -1;
    }

    return 0;
}

//int L3GD20::setControlOne(unsigned char dr, unsigned char bw, bool pd, bool zen, bool xen, bool yen)
int L3GD20::setControlOne()
{
    unsigned char buffer[] = { L3dg20Register::ControlOneAddress, mControl_1 };
    int status = write(mFd, buffer, 2);
    if (status != 2) {
        std::cerr << "L3GD20::" << __func__ << ":" << __LINE__ << " write failed " << status << std::endl;
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
        std::cerr << "L3GD20::" << __func__ << ":" << __LINE__ << " address given " << addr << " is not a valid L3GD20 address" << std::endl;
        std::cerr << "L3GD20::" << __func__ << ":" << __LINE__ << " valid addresses are 0b01101010 and 0b01101011 " << std::endl;
        return -1;
    }

    mAddress = addr;

    return 0;
}

