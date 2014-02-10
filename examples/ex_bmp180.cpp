#include <iostream>

#include <stdio.h>
#include <unistd.h>

#include <bmp180.h>


unsigned char oss_mode = ModeOss3;


int main(int argc, char **argv)
{
    long pressure = 0;
    float temperatur = 0.0;

    /// create bmp180 instance - set device to "/dev/i2c-1"
    Bmp180 bmp180((char*)FirmwareI2CDeviceses::i2c_1);

    /// open device (this will also read calibration data)
    if (bmp180.openDevice() < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " openDevice failed" << std::endl;
        return -1;
    }

    ///  read chip ID (should be 0x55)
    int id = bmp180.readChipId();
    if (id < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " readChipId failed" << std::endl;
        return -2;
    }

    std::cout << "Chip ID 0x" << std::hex << id << std::dec << std::endl;

    /// read temperatur and pressure 10 times
    for (int i = 0; i < 10; i++) {

        /// temperatur...
        if (bmp180.readTemperatur(&temperatur) < 0) {
            std::cerr << __func__ << ":" << __LINE__ << " readRemperatur failed" << std::endl;
            return -1;
        }

        //std::cout << "Temperatur " << std::dec << temperatur << std::endl;

        /// pressure...
        if (bmp180.readPressure(&pressure, oss_mode, true) < 0) {
            std::cerr << __func__ << ":" << __LINE__ << " readPressure failed" << std::endl;
            return 0;
        }

        //std::cout << "Pressure " << std::dec << pressure;
        printf("Temperaur %4.2f Pressure %ld    Altitude %4.2f\n", temperatur, pressure, altitude(pressure, 99700));
    }

    if (bmp180.closeDevice() < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " closeDevice failed" << std::endl;
        return 0;
    }

    return 0;
}
