#include <iostream>
#include <iomanip>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdlib.h>


#include <gpio.h>
#include <ssd1306.h>
#include <bmp180.h>
#include <binary.h>
#include <font.h>


extern unsigned char arduino[1024];


int initDisplay(SSD1306 *ssd1306)
{
    /// need gpio to run the reset on display
    Gpio gpio;

    /// open gpio device (/dev/mem)
    if (gpio.openDevice() < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " openDevice failed" << std::endl;
        return -1;
    }

    int output = GPIO::Gpio24;

    /// toggle reset
    gpio.setGpioDirection(output, GPIO::GpioOutput);
    gpio.setGpio(output, GPIO::GpioOn);                           /// on
    usleep(1000);                                                 /// on for  1 ms
    gpio.setGpio(output, GPIO::GpioOff);                          /// off
    usleep(10000);                                                /// off for 10 ms
    gpio.setGpio(output, GPIO::GpioOn);                           /// switch on and let be on (enabled)

    gpio.closeDevice();

    /// display off
    if (ssd1306->runCommand(Ssd1306DisplayOff) < 0)                      /// 0xae
        return -2;

    /// set display clock div
    if (ssd1306->runCommand(Ssd1306SetDisplayClockDiv) < 0)               /// 0xd5
        return -3;

    /// the clock div
    if (ssd1306->runCommand(0x80) < 0)
        return -4;

    /// set multiplex
    if (ssd1306->runCommand(ssd1306SetMultiplex) < 0)                     /// 0xa8
        return -5;

    /// the multiplex
    if (ssd1306->runCommand(0x3f) < 0)
        return -6;

    /// set display offset
    if (ssd1306->runCommand(Ssd1306SetDisplayOffset) < 0)
        return -7;

    /// the display offset
    if (ssd1306->runCommand(0x0) < 0)
        return -8;


    /// set start line
    if (ssd1306->runCommand(ssd1306SetStartLine | 0x0) < 0)                  /// 0x40
        return -9;

    /// set charge pump
    if (ssd1306->runCommand(ssd1306ChcargePump) < 0)                         /// 0x40
        return -10;

    /// charge pump set to external vcc
    if (ssd1306->runCommand(0x14) < 0)
        return -11;

    /// set memory addressing mode
    if (ssd1306->runCommand(ssd1306MemoryMode) < 0)                          /// 0x20
        return -12;

    /// memory mode set to 0x00
    if (ssd1306->runCommand(0x00) < 0)
        return -13;

    /// set Seg Re Map
    if (ssd1306->runCommand(ssd1306SegReMap) < 0)                          /// 0xA0
        return -14;

    /// set Com Scan to Dec (write from top to bottom)
    if (ssd1306->runCommand(ssd1306ComScanDec) < 0)
        return -15;

    /// set Com Pins
    if (ssd1306->runCommand(Ssd1306SetComPins) < 0)                          /// 0xDA
        return -16;

    /// the com pins
    if (ssd1306->runCommand(0x12) < 0)
        return -17;

    /// set contrast
    if (ssd1306->runCommand(Ssd1306SetContrast) < 0)                          /// 0x81
        return -18;

    /// internal vcc, run contrast 0xCF
    if (ssd1306->runCommand(0xCF) < 0)
        return -19;

    /// set precharge
    if (ssd1306->runCommand(Ssd1306SetPreCharge) < 0)                         /// 0xD9
        return -20;

    /// internal vcc, run precharge 0xF1
    if (ssd1306->runCommand(0xF1) < 0)
        return -21;

    /// set vcomdetect
    if (ssd1306->runCommand(Ssd1306SetVComDetect) < 0)                         /// 0xDB
        return -22;

    /// vcom detect
    if (ssd1306->runCommand(0x40) < 0)
        return -23;

    /// set display on-resume
    if (ssd1306->runCommand(Ssd1306_DisplayAllOnResume) < 0)                    /// 0xA4
        return -24;

#if 1
    /// set normal display
    if (ssd1306->runCommand(Ssd1306NormalDisplay) < 0)                           /// 0xA6
        return -25;
#else
    if (ssd1306->runCommand(Ssd1306InvertDisplay) < 0)
        return -26;
#endif

    /// set normal display
    if (ssd1306->runCommand(Ssd1306DisplayOn) < 0)                               /// 0xAF
        return -27;

    if (ssd1306->clearDisplay() < 0)
        return -28;

    return 0;
}


int timeDateString(unsigned char *buffer)
{
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime (&rawtime);

    memset(buffer, 0, 26);
    int size = snprintf((char*)buffer, 25, "%s", asctime (timeinfo));
    if (size < 0 || size > 25) {
        std::cerr << __func__ << ":" << __LINE__ << "snprintf failed (" << size << ")" << std::endl;
        return -1;
    }

    return size;
}


int main(int argc, char **argv)
{
    std::cout << "ex_ssd1306" << std::endl;

    /// create bmp180 instance - set device to "/dev/i2c-1"
    Bmp180 bmp180((char*)FirmwareI2CDeviceses::i2c_1);

    /// open device (this will also read calibration data)
    if (bmp180.openDevice() < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " openDevice failed" << std::endl;
        return -1;
    }


    /// SSD device
    SSD1306 ssd1306((char*)FirmwareI2CDeviceses::i2c_1);

    /// set i2c address
    if (ssd1306.setAddress(SSD1306Addresses::SSD1306Address1) < 0)
        return 0;

    /// open ssd 1306
    if (ssd1306.openDevice() < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " openDevice failed" << std::endl;
        return 0;
    }

    /// initialize display
    if (initDisplay(&ssd1306) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " initDisplay failed" << std::endl;
        return 0;
    }

    int loop = 5;

    while ( true) {
        int size = 0;
        unsigned char buffer[26];
        long pressure = 0;
        float temperatur = 0.0;
        int oss_mode = ModeOss3;

        /// populate buffer with first part of upper case letters
        memset(buffer, 0, 26);
        size = snprintf((char*)buffer, 25, "BMP 180 data");
        if (size < 0 || size > 25) {
            std::cerr << __func__ << ": " << __LINE__ << "snprintf failed (" << size << ")" << std::endl;
            return -1;
        }

        /// write first part of upper case letters to display
        if (ssd1306.writeLine(0, buffer) < 0) {
            std::cerr << __func__ << ":" << __LINE__ << "writeLine failed" << std::endl;
            return -2;
        }

        if (loop ++ == 5) {
            /// temperatur...
            if (bmp180.readTemperatur(&temperatur) < 0) {
                std::cerr << __func__ << ":" << __LINE__ << " readRemperatur failed" << std::endl;
                return -1;
            }

            /// populate buffer with last part of upper case letters
            memset(buffer, 0, 26);
            size = snprintf((char*)buffer, 25, "Temperature %4.2f c", temperatur);
            if (size < 0 || size > 25) {
                std::cerr << __func__ << ":" << __LINE__ << "snprintf failed (" << size << ")" << std::endl;
                return -1;
            }

            /// write last part of upper case letters
            if (ssd1306.writeLine(2, buffer) < 0) {
                std::cerr << __func__ << ":" << __LINE__ << "writeLine failed" << std::endl;
                return -1;
            }

            /// pressure...
            if (bmp180.readPressure(&pressure, oss_mode, true) < 0) {
                std::cerr << __func__ << ":" << __LINE__ << " readPressure failed" << std::endl;
                return 0;
            }

            /// populate buffer with last part of upper case letters
            memset(buffer, 0, 26);
            size = snprintf((char*)buffer, 25, "Pressure %ld Pa", pressure);
            if (size < 0 || size > 25) {
                std::cerr << __func__ << ":" << __LINE__ << "snprintf failed (" << size << ")" << std::endl;
                return -1;
            }

            /// write last part of upper case letters
            if (ssd1306.writeLine(3, buffer) < 0) {
                std::cerr << __func__ << ":" << __LINE__ << "writeLine failed" << std::endl;
                return -1;
            }

            /// populate buffer with last part of upper case letters
            memset(buffer, 0, 26);
            size = snprintf((char*)buffer, 25, "Altitude %4.2f Meter", altitude(pressure, 99500));
            if (size < 0 || size > 25) {
                std::cerr << __func__ << ":" << __LINE__ << "snprintf failed (" << size << ")" << std::endl;
                return -1;
            }

            /// write last part of upper case letters
            if (ssd1306.writeLine(4, buffer) < 0) {
                std::cerr << __func__ << ":" << __LINE__ << "writeLine failed" << std::endl;
                return -1;
            }
            loop = 0;
        }

        memset(buffer, 0, 26);
        size = snprintf((char*)buffer, 25, "Time and date at exec");
        if (size < 0 || size > 25) {
            std::cerr << __func__ << ":" << __LINE__ << "snprintf failed (" << size << ")" << std::endl;
            return -1;
        }

        if (ssd1306.writeLine(6, buffer) < 0) {
            std::cerr << __func__ << ":" << __LINE__ << "writeLine failed" << std::endl;
            return -1;
        }

        /// get time and date string
        if (timeDateString(buffer) < 0) {
            std::cerr << __func__ << ":" << __LINE__ << "timeDateString failed" << std::endl;
            return 0;
        }

        /// write time and date to display
        if (ssd1306.writeLine(7, buffer) < 0) {
            std::cerr << __func__ << ":" << __LINE__ << "writeLine failed" << std::endl;
            return -1;
        }

        usleep(100000);


    } /// end of while

    ssd1306.closeDevice();

    return 0;
}