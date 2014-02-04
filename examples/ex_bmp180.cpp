#include <iostream>

#include <unistd.h>
#include <stdio.h>

#include <bmp180.h>

int main(int argc, char **argv)
{
    Bmp180 bmp180((char*)FirmwareI2CDeviceses::i2c_1);
    //printf("%s(): address 0x%2x\n", __func__, bmp180.address());

    bmp180.setDebug(false);


    if (bmp180.openDevice() < 0)
        return 0;

    if (bmp180.initialize() < 0)
        return 0;

    bmp180.setDebug(false);

#if 1
    float min_temp = 999999.0;
    float max_temp = 0.0;
    for (int i = 0; i < 255; i++) {
        if (bmp180.readTemperatur() < 0)
            return 0;

        if (bmp180.temperatur() < min_temp)
            min_temp = bmp180.temperatur();
        if (bmp180.temperatur() > max_temp)
            max_temp = bmp180.temperatur();

        printf("%4.2f\t", bmp180.temperatur());
        fflush(0);

        if (i % 20 == 0)
          printf("\n");
    }
    printf("\n");
    printf("min %4.2f max %4.2f avarage %4.2f\n", min_temp, max_temp, (min_temp + max_temp) / 2.0);
#endif

#if 1
    double alt[255];
    double max = 0.0;
    double min = 99999.0;

    for (int i = 0; i < 255; i++) {
        alt[i] = altitude(bmp180.readPressure(), P0AtSea);
        if (alt[i] > max)
            max = alt[i];
        if (alt[i] < min)
            min = alt[i];
        printf("%4.2f\t", alt[i]);
        fflush(0);
        //usleep(10000);
        if (i % 20 == 0)
            printf("\n");
    }
    printf("\n");
    printf("min %4.2f max %4.2f avarage %4.2f\n", min, max, (min + max) / 2.0);
#endif

    bmp180.setDebug(false);

    if (bmp180.closeDevice() < 0)
        return 0;

    return 0;
}
