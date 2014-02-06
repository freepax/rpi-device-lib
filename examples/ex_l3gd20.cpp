#include <iostream>
#include <stdio.h>

#include <l3gd20.h>
#include <binary.h>


int main(int argc, char **argv)
{
    Binary binary;
    L3GD20 l3gd20;

    /// turn debugging on
    l3gd20.setDebug(true);

    printf("I2C Address 0x%02x\n", L3dg20::L3gd20Address1);

    if (l3gd20.setAddress(L3dg20::L3gd20Address1) < 0)
        return 0;

    if (l3gd20.openDevice() < 0)
        return 0;

    /// set the control one register
    if (l3gd20.setControlOne() < 0)
        return 0;

    unsigned char serial = 0;
    if (l3gd20.whoAmI(&serial) < 0)
        return 0;

    binary.printByteAsBinary("Serial number", serial);

    unsigned char temperatur;
    if (l3gd20.readTemperatur(&temperatur) < 0)
        return 0;

    binary.printByteAsBinary("Temperatur ", temperatur);

    short zRotation = -1;
    if (l3gd20.readZrotation(&zRotation) < 0)
        return 0;

    binary.printShortAsBinary("Z Rotation", zRotation);

    std::cout << "Z Rotation " << zRotation << std::endl;

    return 0;
}
