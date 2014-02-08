#include <iostream>
#include <iomanip>

#include <stdio.h>

#include <l3gd20.h>
#include <binary.h>


int main(int argc, char **argv)
{
    Binary binary;

    L3GD20 l3gd20((char*)FirmwareI2CDeviceses::i2c_1);

    /// turn debugging on
    l3gd20.setDebug(true);

    printf("I2C Address 0x%02x\n", L3dg20::L3gd20Address1);

    if (l3gd20.setAddress(L3dg20::L3gd20Address1) < 0)
        return 0;

    if (l3gd20.openDevice() < 0)
        return 0;

    if (l3gd20.writeRegister(L3dg20Register::ControlOneAddress, L3dg20Defaults::ControlOne) < 0)
        return 0;

    if (l3gd20.writeRegister(L3dg20Register::ControlTwoAddress, L3dg20Defaults::ControlTwo) < 0)
        return 0;

    if (l3gd20.writeRegister(L3dg20Register::ControlThreeAddress, L3dg20Defaults::ControlThree) < 0)
        return 0;

    if (l3gd20.writeRegister(L3dg20Register::ControlFourAddress, L3dg20Defaults::ControlFour) < 0)
        return 0;

    if (l3gd20.writeRegister(L3dg20Register::ControlFiveAddress, L3dg20Defaults::ControlFive) < 0)
        return 0;

    /// set the control one register
    if (l3gd20.setControlOne() < 0)
        return 0;

    unsigned char serial = 0;
    if (l3gd20.whoAmI(&serial) < 0)
        return 0;

    printf("Serial number 0x%02x\n", serial);
    //std::cout << "Serial number  0x" << std::hex << std::setw(4) << std::setfill('0') << serial << std::dec << std::endl;
    //binary.printByteAsBinary("Serial number", serial);

    unsigned char temperatur;
    if (l3gd20.readTemperatur(&temperatur) < 0)
        return 0;

    //std::cout << "Temperatur 0x" << std::hex << temperatur << std::dec << std::endl;
    printf("Temperatur 0x%02x\n", temperatur);
    binary.printByteAsBinary("Temperatur ", temperatur);

    short zRotation = -1;
    if (l3gd20.readZrotation(&zRotation) < 0)
        return 0;

    binary.printShortAsBinary("Z Rotation", zRotation);

    std::cout << "Z Rotation " << zRotation << std::endl;

    return 0;
}
