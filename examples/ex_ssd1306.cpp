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

    /// toggle reset
    gpio.setGpioDirection(GPIO::Gpio8, GPIO::GpioOutput);
    gpio.setGpio(GPIO::Gpio8, GPIO::GpioOn);                           /// on
    usleep(1000);                                                      /// on for  1 ms
    gpio.setGpio(GPIO::Gpio8, GPIO::GpioOff);                          /// off
    usleep(10000);                                                     /// off for 10 ms
    gpio.setGpio(GPIO::Gpio8, GPIO::GpioOn);                           /// switch on and let be on (enabled)

    gpio.closeDevice();

    /// display off
    if (ssd1306->runCommand(Ssd1306DisplayOff) < 0)
        return -2;

    /// set display clock div
    if (ssd1306->runCommand(Ssd1306SetDisplayClockDiv) < 0)
        return -3;

    /// the clock div
    if (ssd1306->runCommand(0x80) < 0)
        return -4;

    /// set multiplex
    if (ssd1306->runCommand(ssd1306SetMultiplex) < 0)
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
    if (ssd1306->runCommand(ssd1306SetStartLine | 0x0) < 0)
        return -9;

    /// set charge pump
    if (ssd1306->runCommand(ssd1306ChcargePump) < 0)
        return -10;

    /// charge pump set to external vcc
    if (ssd1306->runCommand(0x14) < 0)
        return -11;

    /// set memory addressing mode
    if (ssd1306->runCommand(ssd1306MemoryMode) < 0)
        return -12;

    /// memory mode set to 0x00
    if (ssd1306->runCommand(0x00) < 0)
        return -13;

    /// set Seg Re Map
    if (ssd1306->runCommand(ssd1306SegReMap) < 0)
        return -14;

    /// set Com Scan to Dec (write from top to bottom)
    if (ssd1306->runCommand(ssd1306ComScanDec) < 0)
        return -15;

    /// set Com Pins
    if (ssd1306->runCommand(Ssd1306SetComPins) < 0)
        return -16;

    /// the com pins
    if (ssd1306->runCommand(0x12) < 0)
        return -17;

    /// set contrast
    if (ssd1306->runCommand(Ssd1306SetContrast) < 0)
        return -18;

    /// internal vcc, run contrast 0xCF
    if (ssd1306->runCommand(0xCF) < 0)
        return -19;

    /// set precharge
    if (ssd1306->runCommand(Ssd1306SetPreCharge) < 0)
        return -20;

    /// internal vcc, run precharge 0xF1
    if (ssd1306->runCommand(0xF1) < 0)
        return -21;

    /// set vcomdetect
    if (ssd1306->runCommand(Ssd1306SetVComDetect) < 0)
        return -22;

    /// vcom detect
    if (ssd1306->runCommand(0x40) < 0)
        return -23;

    /// set display on-resume
    if (ssd1306->runCommand(Ssd1306_DisplayAllOnResume) < 0)
        return -24;

#if 1
    /// set normal display
    if (ssd1306->runCommand(Ssd1306NormalDisplay) < 0)
        return -25;
#else
    if (ssd1306->runCommand(Ssd1306InvertDisplay) < 0)
        return -26;
#endif

    /// set normal display
    if (ssd1306->runCommand(Ssd1306DisplayOn) < 0)
        return -27;

    /// clear display
    if (ssd1306->clearDisplay() < 0)
        return -28;

    return 0;
}


/// write image (1024 bytes) to display
int writeImage(SSD1306* ssd1306, unsigned char image[1024])
{
    /// write image (1024 bytes) to display
    if (ssd1306->writeImage(image) < 0)
        return -1;

    return 0;
}


int writeByteTest(SSD1306 *ssd1306)
{
    /// for all 8 lines on display
    for (int line = 0; line < 8; line++) {

        /// 128 pixels across - each line is 8 byte hi
        for (int position = 0; position < 128; position++) {

            //std::cout << "line " << line << " position " << position << std::endl;

            /// write byte
            if (ssd1306->writeByte(line, position, 0xff) < 0) {
                std::cerr << __func__ << ":" << __LINE__ << " writeByte failed" << std::endl;
                return -1;
            }

            usleep(1000);
        } /// position
    } /// line

    ssd1306->clearDisplay();
}

/// create time and date string
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


/// create network strings and write them to the display
int networkPage(SSD1306 *ssd1306)
{
    unsigned char buffer[26];
    struct ifaddrs *ifaddr, *ifa;
    char host[NI_MAXHOST];
    int size = 0;

    /// prepare buffer for write
    memset(buffer, 0, 26);
    size = snprintf((char*)buffer, 25, "N E T W O R K ");
    if (size < 0 || size > 25) {
        std::cerr << __func__ << ":" << __LINE__ << "snprintf failed (" << size << ")" << std::endl;
        return -1;
    }

    /// write buffer to display
    if (ssd1306->writeLine(0, buffer) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << "writeLine failed" << std::endl;
        return -2;
    }

    /// get addresses
    if (getifaddrs(&ifaddr) == -1) {
        std::cerr << __func__ << ":" << __LINE__ << "getifaddrs failed" << std::endl;
        return -3;
    }

    int line = 4;

    /// loop over ifa list of devices and get hold of interfaces which is up
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        int family = ifa->ifa_addr->sa_family;

        /// Only INET interfaces
        if (family == AF_INET) {
            int s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            if (s != 0) {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                return -4;
            }

            //std::cout << "Interface " << ifa->ifa_name << " address " << host << std::endl;

            /// prepare buffer for write
            memset(buffer, 0, 26);
            size = snprintf((char*)buffer, 25, "%s %s", ifa->ifa_name, host);
            if (size > 25) {
                std::cerr << __func__ << ":" << __LINE__ << "snprintf failed (" << size << ")" << std::endl;
                return -5;
            }

            /// write buffer to display
            if (ssd1306->writeLine(line++, buffer) < 0) {
                std::cerr << __func__ << ":" << __LINE__ << "writeLine failed" << std::endl;
                return -6;
            }
        }
    }

    return 0;
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
    if (ssd1306.openDevice() < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " openDevice failed" << std::endl;
        return 0;
    }

    /// initialize display
    if (initDisplay(&ssd1306) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " initDisplay failed" << std::endl;
        return 0;
    }

    while ( true) {
        int size = 0;
        unsigned char buffer[26];

        /// populate buffer with first part of upper case letters
        memset(buffer, 0, 26);
        size = snprintf((char*)buffer, 25, "A B C D E F G H I J K L M");
        if (size < 0 || size != 25) {
            std::cerr << __func__ << ":" << __LINE__ << "snprintf failed (" << size << ")" << std::endl;
            return -1;
        }

        /// write first part of upper case letters to display
        if (ssd1306.writeLine(0, buffer) < 0) {
            std::cerr << __func__ << ":" << __LINE__ << "writeLine failed" << std::endl;
            return -2;
        }

        /// populate buffer with last part of upper case letters
        memset(buffer, 0, 26);
        size = snprintf((char*)buffer, 25, "N O P Q R S T U V W X Y Z");
        if (size < 0 || size != 25) {
            std::cerr << __func__ << ":" << __LINE__ << "snprintf failed (" << size << ")" << std::endl;
            return -1;
        }

        /// write last part of upper case letters
        if (ssd1306.writeLine(1, buffer) < 0) {
            std::cerr << __func__ << ":" << __LINE__ << "writeLine failed" << std::endl;
            return -1;
        }

        /// populate buffer with numbers
        memset(buffer, 0, 26);
        size = snprintf((char*)buffer, 25, "0 1 2 3 4 5 6 7 8 9 _ -  ");
        if (size < 0 || size != 25) {
            std::cerr << __func__ << ":" << __LINE__ << "snprintf failed (" << size << ")" << std::endl;
            return -1;
        }

        /// write numbers to display
        if (ssd1306.writeLine(2, buffer) < 0) {
            std::cerr << __func__ << ":" << __LINE__ << "writeLine failed" << std::endl;
            return -1;
        }

        /// populate buffer with some symbols
        memset(buffer, 0, 26);
        size = snprintf((char*)buffer, 25, "! \" # % & / ( ) = ? @ $  ");
        if (size < 0 || size != 25) {
            std::cerr << __func__ << ":" << __LINE__ << "snprintf failed (" << size << ")" << std::endl;
            return -1;
        }

        /// write symbols to display
        if (ssd1306.writeLine(3, buffer) < 0) {
            std::cerr << __func__ << ":" << __LINE__ << "writeLine failed" << std::endl;
            return -1;
        }

        /// populate buffer with first part of lower case letters
        memset(buffer, 0, 26);
        size = snprintf((char*)buffer, 25, "a b c d e f g h i j k l m");
        if (size < 0 || size != 25) {
            std::cerr << __func__ << ":" << __LINE__ << "snprintf failed (" << size << ")" << std::endl;
            return -1;
        }

        /// write first part of lower case letters
        if (ssd1306.writeLine(4, buffer) < 0) {
            std::cerr << __func__ << ":" << __LINE__ << "writeLine failed" << std::endl;
            return -1;
        }

        /// populate buffer with last part of lower case letters
        memset(buffer, 0, 26);
        size = snprintf((char*)buffer, 25, "n o p q r s t u v w x y z");
        if (size < 0 || size != 25) {
            std::cerr << __func__ << ":" << __LINE__ << "snprintf failed (" << size << ")" << std::endl;
            return -1;
        }

        /// write last part of lower case letters
        if (ssd1306.writeLine(5, buffer) < 0) {
            std::cerr << __func__ << ":" << __LINE__ << "writeLine failed" << std::endl;
            return -1;
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

        usleep(2000000);

        /// clear display
        if (ssd1306.clearDisplay() < 0) {
            std::cerr << __func__ << ":" << __LINE__ << " clearDisplay failed" << std::endl;
            return 0;
        }

#if 0
        /// run byte test
        if (writeByteTest(&ssd1306) < 0) {
            std::cerr << __func__ << ":" << __LINE__ << " writeLineTest failed with status " << status << std::endl;
            return 0;
        }
        ssd1306.clearDisplay()();
#endif

        /// write image (adruino) to display
        if (writeImage(&ssd1306, arduino) < 0) {
            std::cerr << __func__ << ":" << __LINE__ << " writeImage failed" << std::endl;
            return 0;
        }

        usleep(1000000);

        /// clear display
        if (ssd1306.clearDisplay() < 0) {
            std::cerr << __func__ << ":" << __LINE__ << " clearDisplay failed" << std::endl;
            return 0;
        }

        /// show network information on display
        if (networkPage(&ssd1306) < 0) {
            std::cerr << __func__ << ":" << __LINE__ << " networkPage failed" << std::endl;
            return 0;
        }

        usleep(2000000);

        /// clear display
        if (ssd1306.clearDisplay() < 0) {
            std::cerr << __func__ << ":" << __LINE__ << " clearDisplay failed" << std::endl;
            return 0;
        }

    } /// end of while

    ssd1306.closeDevice();

    return 0;
}
