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


long p0 = 99370;
static const int samples = 1;


int initDisplay(SSD1306 *ssd1306)
{
    /// need gpio to toggle the reset pin on the display
    Gpio gpio;

    /// open gpio device (/dev/mem)
    if (gpio.openDevice() < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " openDevice failed" << std::endl;
        return -1;
    }

    int output = GPIO::Gpio24;

    /// toggle reset
    gpio.setGpioDirection(output, GPIO::GpioOutput);              /// set gpio direction to output
    gpio.setGpio(output, GPIO::GpioOn);                           /// switch on
    usleep(1000);                                                 /// sleep 1 ms
    gpio.setGpio(output, GPIO::GpioOff);                          /// switch off
    usleep(10000);                                                /// sleep 10 ms
    gpio.setGpio(output, GPIO::GpioOn);                           /// switch on and let be on (enabled)

    gpio.closeDevice();

    /// display off
    if (ssd1306->runCommand(Ssd1306DisplayOff) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -2;
    }

    /// set display clock div
    if (ssd1306->runCommand(Ssd1306SetDisplayClockDiv) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -3;
    }

    /// the clock div
    if (ssd1306->runCommand(0x80) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -4;
    }

    /// set multiplex
    if (ssd1306->runCommand(ssd1306SetMultiplex) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -5;
    }

    /// the multiplex
    if (ssd1306->runCommand(0x3f) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -6;
    }

    /// set display offset
    if (ssd1306->runCommand(Ssd1306SetDisplayOffset) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -7;
    }

    /// the display offset
    if (ssd1306->runCommand(0x0) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -8;
    }


    /// set start line
    if (ssd1306->runCommand(ssd1306SetStartLine | 0x0) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -9;
    }

    /// set charge pump
    if (ssd1306->runCommand(ssd1306ChcargePump) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -10;
    }

    /// charge pump set to external vcc
    if (ssd1306->runCommand(0x14) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -11;
    }

    /// set memory addressing mode
    if (ssd1306->runCommand(ssd1306MemoryMode) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -12;
    }

    /// memory mode set to 0x00
    if (ssd1306->runCommand(0x00) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -13;
    }

    /// set Seg Re Map
    if (ssd1306->runCommand(ssd1306SegReMap) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -14;
    }

    /// set Com Scan to Dec (write from top to bottom)
    if (ssd1306->runCommand(ssd1306ComScanDec) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -15;
    }

    /// set Com Pins
    if (ssd1306->runCommand(Ssd1306SetComPins) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -16;
    }

    /// the com pins
    if (ssd1306->runCommand(0x12) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -17;
    }

    /// set contrast
    if (ssd1306->runCommand(Ssd1306SetContrast) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -18;
    }

    /// internal vcc, run contrast 0xCF
    if (ssd1306->runCommand(0xCF) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -19;
    }

    /// set precharge
    if (ssd1306->runCommand(Ssd1306SetPreCharge) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -20;
    }

    /// internal vcc, run precharge 0xF1
    if (ssd1306->runCommand(0xF1) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -21;
    }

    /// set vcomdetect
    if (ssd1306->runCommand(Ssd1306SetVComDetect) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -22;
    }

    /// vcom detect
    if (ssd1306->runCommand(0x40) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -23;
    }

    /// set display on-resume
    if (ssd1306->runCommand(Ssd1306DisplayAllOnResume) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -24;
    }

#if 1
    /// set normal display
    if (ssd1306->runCommand(Ssd1306NormalDisplay) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -25;
    }
#else
    if (ssd1306->runCommand(Ssd1306InvertDisplay) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -26;
    }
#endif

    /// set normal display
    if (ssd1306->runCommand(Ssd1306DisplayOn) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " runCommand failed" << std::endl;
        return -27;
    }

    /// clear display
    if (ssd1306->clearDisplay() < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " clearDisplay failed" << std::endl;
        return -28;
    }

    return 0;
}

/// create network string and write it to the display
int writeNetworkLine(SSD1306 *ssd1306, int line)
{
    unsigned char buffer[25];
    struct ifaddrs *ifaddr, *ifa;
    char host[NI_MAXHOST];
    int size = 0;
    bool ip_found = false;


    /// get addresses
    if (getifaddrs(&ifaddr) == -1) {
        std::cerr << __func__ << ":" << __LINE__ << "getifaddrs failed" << std::endl;
        return -3;
    }

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

            /// skip loop back
            if (strcmp(ifa->ifa_name, "lo") != 0) {
                ip_found = true;

                //std::cout << "Interface " << ifa->ifa_name << " address " << host << std::endl;

                /// clear and populate buffer
                memset(buffer, 0, 25);
                size = snprintf((char*)buffer, 25, "%s %s", ifa->ifa_name, host);
                if (size < 0 || size > 25) {
                    std::cerr << __func__ << ":" << __LINE__ << "snprintf failed (" << size << ")" << std::endl;
                    return -5;
                }

                /// write buffer to display
                if (ssd1306->writeLine(line, buffer) < 0) {
                    std::cerr << __func__ << ":" << __LINE__ << "writeLine failed" << std::endl;
                    return -6;
                }
            }
        } /// if family == AF_INET
    } /// for loop

    /// clear line if no network is found
    if (ip_found == false) {
        if (ssd1306->clearLine(line) < 0) {
            std::cerr << __func__ << ":" << __LINE__ << "clearLine failed" << std::endl;
            return -1;
        }
    }

    return 0;
}


/// create a string containing date and time
int timeDateString(unsigned char *buffer)
{
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime (&rawtime);

    memset(buffer, 0, 25);
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

    std::cout << "argc " << argc << std::endl;
    if (argc == 2) {
        p0 = atol(argv[1]);
        std::cout << "P0 set to " << std::dec << p0 << std::endl;
    }

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

    while (true) {
        int size = 0;
        unsigned char buffer[26];
        long pressure[samples];
        float temperature = 0.0;
        int oss_mode = ModeOss3;

        /// don't read temperature and pressure too often
        if (loop ++ == 5) {
            /// temperature...
            if (bmp180.readTemperature(&temperature) < 0) {
                std::cerr << __func__ << ":" << __LINE__ << " readRemperatur failed" << std::endl;
                return -1;
            }

            /// populate buffer with temperature reading
            memset(buffer, 0, 26);
            size = snprintf((char*)buffer, 25, "Temperature %4.2f c", temperature);
            if (size < 0 || size > 25) {
                std::cerr << __func__ << ":" << __LINE__ << "snprintf failed (" << size << ")" << std::endl;
                return -1;
            }

            /// write temperature reading to display
            if (ssd1306.writeLine(2, buffer) < 0) {
                std::cerr << __func__ << ":" << __LINE__ << "writeLine failed" << std::endl;
                return -1;
            }

            /// populate buffer with P0 pressure
            memset(buffer, 0, 26);
            size = snprintf((char*)buffer, 25, "P0          %ld Pa", p0);
            if (size < 0 || size > 25) {
                std::cerr << __func__ << ":" << __LINE__ << "snprintf failed (" << size << ")" << std::endl;
                return -1;
            }

            /// write p0 pressure to display
            if (ssd1306.writeLine(3, buffer) < 0) {
                std::cerr << __func__ << ":" << __LINE__ << "writeLine failed" << std::endl;
                return -1;
            }

            /// read pressure
            if (bmp180.readPressure(pressure, oss_mode, samples, true) < 0) {
                std::cerr << __func__ << ":" << __LINE__ << " readPressure failed" << std::endl;
                return 0;
            }

            long pa = 0;
            for (int i = 0; i < samples; i++)
                pa += pressure[i];
            pa /= samples;
            double alt = 98.0;
            printf("Calculated pressure %4.2f (altitude %4.2f)\n", pressure_at_sea(pa, alt), alt);

            /// populate buffer with pressure reading
            memset(buffer, 0, 26);
            size = snprintf((char*)buffer, 25, "Pressure    %ld Pa", pa);
            if (size < 0 || size > 25) {
                std::cerr << __func__ << ":" << __LINE__ << "snprintf failed (" << size << ")" << std::endl;
                return -1;
            }

            /// write pressure reading to display
            if (ssd1306.writeLine(4, buffer) < 0) {
                std::cerr << __func__ << ":" << __LINE__ << "writeLine failed" << std::endl;
                return -1;
            }

            /// populate buffer with altitude
            memset(buffer, 0, 26);
            size = snprintf((char*)buffer, 25, "Altitude %4.2f Meter", altitude(pressure[0], p0));
            if (size < 0 || size > 25) {
                std::cerr << __func__ << ":" << __LINE__ << "snprintf failed (" << size << ")" << std::endl;
                return -1;
            }

            /// write altitude to display
            if (ssd1306.writeLine(5, buffer) < 0) {
                std::cerr << __func__ << ":" << __LINE__ << "writeLine failed" << std::endl;
                return -1;
            }

            /// write wlan ip on display
            if (writeNetworkLine(&ssd1306, 0) < 0) {
                std::cerr << __func__ << ":" << __LINE__ << "writeNetworkLine failed" << std::endl;
                return -1;
            }
            loop = 0;
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

        /// sleep 10 msec
        usleep(100000);


    } /// end of while

    ssd1306.closeDevice();

    return 0;
}
