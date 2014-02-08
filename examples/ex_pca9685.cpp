#include <iostream>

#include <stdlib.h>
#include <unistd.h>

/// pca9685 headers
#include <pca9685.h>


int main(int argc, char **argv)
{
    /// create a PCA9685 instance - make sure to use the right device ("/dev/i2c-1")
    PCA9685 pca9685((char*)FirmwareI2CDeviceses::i2c_1);

    int status = pca9685.openDevice();
    if (status < 0) {
        std::cerr << __func__  << ":" << __LINE__ << " failed to open pca9685"<< std::endl;
        return 0;
    }

    status = pca9685.setOscillatorFrequency(25);
    if (status < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " failed to set oscillator frequency" << std::endl;
        pca9685.closeDevice();
        return 0;
    }

    status = pca9685.setFrequency(60);
    if (status < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " failed to set frequency" << std::endl;
        pca9685.closeDevice();
        return 0;
    }

    for (int i = 0; i < 10; i++) {
        for (int pin = 0; pin < 16; pin++) {
            status = pca9685.writeOutput(pin, 4095, 0);
            if (status < 0) {
                std::cerr << __func__  << ":" << __LINE__ << " failed to read pca9685"<< std::endl;
                pca9685.closeDevice();
                return 0;
            }
        }

        usleep(600000);

        for (int pin = 0; pin < 16; pin++) {
            status = pca9685.writeOutput(pin, 0, 0);
            if (status < 0) {
                std::cerr << __func__  << ":" << __LINE__ << " failed to read pca9685"<< std::endl;
                pca9685.closeDevice();
                return 0;
            }
        }

        usleep(600000);
    }

    return 0;

    for (int i = 0; i < 10; i++) {

        for (int j = 1; j < 17; j++) {
            status = pca9685.writeOutput(j, PCA9685Constants::Lo, PCA9685Constants::Off, 0b00000000);
            if (status < 0) {
                std::cerr << __func__  << ":" << __LINE__ << " failed to read pca9685"<< std::endl;
                pca9685.closeDevice();
                return 0;
            }

            status = pca9685.writeOutput(j, PCA9685Constants::Lo, PCA9685Constants::On, 0b00000000);
            if (status < 0) {
                std::cerr << __func__  << ":" << __LINE__ << " failed to read pca9685"<< std::endl;
                pca9685.closeDevice();
                return 0;
            }

            status = pca9685.writeOutput(j, PCA9685Constants::Hi, PCA9685Constants::Off, 0b00000000);
            if (status < 0) {
                std::cerr << __func__  << ":" << __LINE__ << " failed to read pca9685"<< std::endl;
                pca9685.closeDevice();
                return 0;
            }

            status = pca9685.writeOutput(j, PCA9685Constants::Hi, PCA9685Constants::On, 0b00000000);
            if (status < 0) {
                std::cerr << __func__  << ":" << __LINE__ << " failed to read pca9685"<< std::endl;
                pca9685.closeDevice();
                return 0;
            }
        }

        usleep(600000);

        for (int j = 1; j < 17; j++) {
            status = pca9685.writeOutput(j, PCA9685Constants::Lo, PCA9685Constants::Off, 0b11111111);
            if (status < 0) {
                std::cerr << __func__  << ":" << __LINE__ << " failed to read pca9685"<< std::endl;
                pca9685.closeDevice();
                return 0;
            }


#if 0
            status = pca9685.writeOutput(j, PCA9685Constants::Lo, PCA9685Constants::On, 0b11111111);
            if (status < 0) {
                std::cerr << __func__  << ":" << __LINE__ << " failed to read pca9685"<< std::endl;
                pca9685.closeDevice();
                return 0;
            }
#endif
#if 0
            status = pca9685.writeOutput(j, PCA9685Constants::Hi, PCA9685Constants::Off, 0b11111111);
            if (status < 0) {
                std::cerr << __func__  << ":" << __LINE__ << " failed to read pca9685"<< std::endl;
                pca9685.closeDevice();
                return 0;
            }
#endif
#if 0
            status = pca9685.writeOutput(j, PCA9685Constants::Hi, PCA9685Constants::On, 0b11111111);
            if (status < 0) {
                std::cerr << __func__  << ":" << __LINE__ << " failed to read pca9685"<< std::endl;
                pca9685.closeDevice();
                return 0;
            }
#endif

        }

        usleep(600000);
    }

    pca9685.closeDevice();

    return 0;
}
