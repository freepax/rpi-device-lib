#include <iostream>

#include <unistd.h>
#include <stdio.h>

#include <bmp180.h>


unsigned char oss_mode = ModeOss0;


int main(int argc, char **argv)
{
    long pressure = 0;
    float temperatur = 0.0;

    Bmp180 bmp180((char*)FirmwareI2CDeviceses::i2c_1);

    if (bmp180.openDevice() < 0)
        return -1;

    int id = bmp180.readChipId();
    if (id < 0)
        return -2;

    std::cout << "Chip ID 0x" << std::hex << id << std::endl;
    printf("Chip ID: 0x%02x\n", (char)id);

    for (int i = 0; i < 10; i++) {
        if (bmp180.readTemperatur(&temperatur) < 0)
            return -1;

        std::cout << "Temperatur " << temperatur << std::endl;

        if (bmp180.readPressure(&pressure, oss_mode, true) < 0)
            return 0;
        std::cout << "Pressure " << std::dec << pressure << " Altitude " << altitude(pressure, 99750) << std::endl;
    }

    if (bmp180.closeDevice() < 0)
        return 0;

    return 0;
}
