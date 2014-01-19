#include <iostream>

#include <l3gd20.h>


L3GD20::L3GD20(char *device, int address, bool debug) : Firmware_I2C(device, address, debug),
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

