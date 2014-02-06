#include <iostream>

#include <stdio.h>
#include <unistd.h>

#include <gpio.h>
#include <ssd1306.h>
#include <binary.h>


int main(int argc, char **argv)
{
    std::cout << "ex_ssd1306" << std::endl;

    /// SSD device
    SSD1306 ssd1306((char*)FirmwareI2CDeviceses::i2c_1);

    /// turn debugging on
    ssd1306.setDebug(true);

    /// set i2c address
    if (ssd1306.setAddress(SSD1306Addresses::SSD1306Address1) < 0)
        return 0;

    //printf("Address 0x%x\n", SSD1306Addresses::SSD1306Address1);

    //std::cout << __func__ << "(): descriptor " << ssd1306.fd() << std::endl << std::endl;

    /// open ssd 1306
    if (ssd1306.openDevice() < 0)
        return 0;

    //std::cout << __func__ << "(): descriptor " << ssd1306.fd() << std::endl << std::endl;

    /// need gpio to run the reset on display
    Gpio gpio;

    /// open gpio device (/dev/mem)
    if (gpio.openDevice() < 0) {
        std::cerr << __func__ << "(): openDevice failed" << std::endl;
        return 0;
    }

    /// toggle reset
    gpio.setGpioDirection(GPIO::Gpio24, GPIO::GpioOutput);
    gpio.setGpio(GPIO::Gpio24, GPIO::GpioOn);                           /// on
    usleep(1000);                                                       /// on for  1 ms
    gpio.setGpio(GPIO::Gpio24, GPIO::GpioOff);                          /// off
    usleep(10000);                                                      /// off for 10 ms
    gpio.setGpio(GPIO::Gpio24, GPIO::GpioOn);                           /// switch on and let be on (enabled)
    //usleep(100);                                                        /// on for  100 us

    /// display off
    if (ssd1306.runCommand(Ssd1306DisplayOff) < 0)                      /// 0xae
        return 0;

    /// set display clock div
    if (ssd1306.runCommand(Ssd1306SetDisplayClockDiv) < 0)               /// 0xd5
        return 0;

    /// the clock div
    if (ssd1306.runCommand(0x80) < 0)
        return 0;

    /// set multiplex
    if (ssd1306.runCommand(ssd1306SetMultiplex) < 0)                     /// 0xa8
        return 0;

    /// the multiplex
    if (ssd1306.runCommand(0x3f) < 0)
        return 0;

    /// set display offset
    if (ssd1306.runCommand(Ssd1306SetDisplayOffset) < 0)                    /// 0x3d
        return 0;

    /// the display offset
    if (ssd1306.runCommand(0x0) < 0)
        return 0;


    /// set start line
    if (ssd1306.runCommand(ssd1306SetStartLine | 0x0) < 0)                  /// 0x40
        return 0;

    /// set charge pump
    if (ssd1306.runCommand(ssd1306ChcargePump) < 0)                         /// 0x40
        return 0;


    /// external vcc
    if (ssd1306.runCommand(0x14) < 0)
        return 0;

    /// set memory mode
    if (ssd1306.runCommand(ssd1306MemoryMode) < 0)                          /// 0x20
        return 0;

    /// the memory mode
    if (ssd1306.runCommand(0x0) < 0)
        return 0;

    /// set Seg Re Map
    if (ssd1306.runCommand(ssd1306SegReMap) < 0)                          /// 0xA0
        return 0;

    /// set Com Scan Dec
    if (ssd1306.runCommand(ssd1306ComScanDec) < 0)                          /// 0xC8
        return 0;


    /// set Com Pins
    if (ssd1306.runCommand(Ssd1306SetComPins) < 0)                          /// 0xDA
        return 0;

    /// the com pins
    if (ssd1306.runCommand(0x12) < 0)
        return 0;

    /// set contrast
    if (ssd1306.runCommand(Ssd1306SetContrast) < 0)                          /// 0x81
        return 0;

    /// internal vcc, run contrast 0xCF
    if (ssd1306.runCommand(0xCF) < 0)
        return 0;

    /// set precharge
    if (ssd1306.runCommand(Ssd1306SetPreCharge) < 0)                         /// 0xD9
        return 0;

    /// internal vcc, run precharge 0xF1
    if (ssd1306.runCommand(0xF1) < 0)
        return 0;

    /// set vcomdetect
    if (ssd1306.runCommand(Ssd1306SetVComDetect) < 0)                         /// 0xDB
        return 0;

    /// vcom detect
    if (ssd1306.runCommand(0x40) < 0)
        return 0;

    /// set display on-resume
    if (ssd1306.runCommand(Ssd1306_DisplayAllOnResume) < 0)                    /// 0xA4
        return 0;

    /// set normal display
    if (ssd1306.runCommand(Ssd1306NormalDisplay) < 0)                           /// 0xA6
        return 0;

    /// set normal display
    if (ssd1306.runCommand(Ssd1306DisplayOn) < 0)                               /// 0xAF
        return 0;


#if 1
    char buffer[1024];
    for (int i = 0; i < 1024; i++)
        buffer[i] = 0b10101010;

    if (ssd1306.writeData(buffer, 10) < 0) {
        std::cerr << __func__ << "(): writeData failed" << std::endl;
        ssd1306.closeDevice();
        return 0;
    }
#endif

    ssd1306.closeDevice();

    return 0;
}
