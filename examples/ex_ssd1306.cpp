#include <iostream>
#include <iomanip>

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <gpio.h>
#include <ssd1306.h>
#include <binary.h>
#include <arial_bold.h>


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

    /// toggle reset
    gpio.setGpioDirection(GPIO::Gpio8, GPIO::GpioOutput);
    gpio.setGpio(GPIO::Gpio8, GPIO::GpioOn);                           /// on
    usleep(1000);                                                       /// on for  1 ms
    gpio.setGpio(GPIO::Gpio8, GPIO::GpioOff);                          /// off
    usleep(10000);                                                      /// off for 10 ms
    gpio.setGpio(GPIO::Gpio8, GPIO::GpioOn);                           /// switch on and let be on (enabled)

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
    if (ssd1306->runCommand(Ssd1306SetDisplayOffset) < 0)                    /// 0x3d
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

    if (ssd1306->clear() < 0)
        return -28;

    return 0;
}


int writeImage(SSD1306* ssd1306, unsigned char image[1023])
{
    int pointer = 0;

    for (unsigned char line = 0; line < 8; line++) {

        unsigned char data[128];
        memset(data, 0, 128);

        for (int i = 0; i < 128; i++)
            data[i] = image[pointer++];

        if (ssd1306->writeLine(line, data) < 0)
            return -1;
    }

    return 0;
}


int writeLineTest(SSD1306* ssd1306)
{
    for (unsigned char line = 0; line < 8; line++) {

        unsigned char data[128];
        memset(data, 1, 128);

        for (int i = 0; i < 128; i++)
            data[i] = 0b11111111;

        if (ssd1306->writeLine(line, data) < 0)
            return -1;

        usleep(300000);

        ssd1306->clear();
    }

    return 0;
}

#include <binary.h>
int writeByteTest(SSD1306 *ssd1306)
{
    for (int repeate = 0; repeate < 3; repeate++) {
        for (int line = 0; line < 8; line++) {
            for (int position = 0; position < 16; position++) {

                /// byte
                unsigned char buffer[] = { 0xff };
                std::cout <<  "line " << line << " position " << position << std::endl;

                /// write byte
                if (ssd1306->writeByte(line, position, buffer[0]) < 0) {
                    std::cerr << __func__ << ":" << __LINE__ << " writeByte failed" << std::endl;
                    return -1;
                }
                usleep(10000);
            } /// position
        } /// line

        usleep(300000);
        ssd1306->clear();
        std::cout << "repeate " << std::dec << repeate << std::endl;

    } /// repeate

    ssd1306->clear();
}

int main(int argc, char **argv)
{
    std::cout << "ex_ssd1306" << std::endl;

    /// SSD device
    SSD1306 ssd1306((char*)FirmwareI2CDeviceses::i2c_1);

    /// set i2c address
    if (ssd1306.setAddress(SSD1306Addresses::SSD1306Address1) < 0)
        return 0;

    /// open ssd 1306
    if (ssd1306.openDevice() < 0)
        return 0;

    int status = initDisplay(&ssd1306);
    if (status < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " initDisplay failed with status " << status << std::endl;
        return 0;
    }

#if 0
    if (writeLineTest(&ssd1306) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " writeLineTest failed with status " << status << std::endl;
        return 0;
    }
#endif


    ssd1306.clear();

    if (writeByteTest(&ssd1306) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " writeLineTest failed with status " << status << std::endl;
        return 0;
    }


    int fnt = 0;
    while ( true) {
        unsigned char line[128];

        for (int i = 0; i < 8; i++) {

            for (int j = 0; j < 128; j++) {
                line[j] = font[fnt];
                if (++fnt > 1023) fnt = 0;
            }

            if (ssd1306.writeLine(i, line) < 0) {
                std::cerr << __func__ << ":" << __LINE__ << " writeImage failed" << std::endl;
                return 0;
            }
        }

        /// display characters a few seconds
        usleep(2000000);
        ssd1306.clear();

        if (writeImage(&ssd1306, arduino) < 0) {
            std::cerr << __func__ << ":" << __LINE__ << " writeImage failed" << std::endl;
            return 0;
        }

        /// display image a few seconds
        usleep(2000000);
        ssd1306.clear();
    } /// end of while

    ssd1306.closeDevice();

    return 0;
}
