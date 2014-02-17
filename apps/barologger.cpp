#include <iostream>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include <bmp180.h>


static const int samples = 1;
static const int fileNameSize = 20;

unsigned char oss_mode = ModeOss3;


/// create file name string from date and time
int fileNameString(char *buffer)
{
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime (&rawtime);

    memset(buffer, 0, fileNameSize);
    int size = sprintf((char*)buffer, "%04d-%02d-%02d-%02d-baro.log", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour);
    if (size < 0) {
        std::cerr << __func__ << ":" << __LINE__ << "snprintf failed (" << size << ")" << std::endl;
        return -1;
    }

    return size;
}


/// create file name string from date and time
int logTimeString(char *buffer)
{
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime (&rawtime);

    memset(buffer, 0, fileNameSize);
    int size = sprintf((char*)buffer, "%04d-%02d-%02d %02d:%02d:%02d", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    if (size < 0) {
        std::cerr << __func__ << ":" << __LINE__ << "snprintf failed (" << size << ")" << std::endl;
        return -1;
    }

    return size;
}

int openFile()
{
    char filename[fileNameSize];

    if (fileNameString(filename) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " fileNameString failed" << std::endl;
        return 0;
    }

    int fd = open(filename, O_CREAT | O_APPEND | O_WRONLY);
    if (fd < 0)
        std::cerr << __func__ << ":" << __LINE__ << " open failed" << std::endl;

    return fd;
}

int main(int argc, char **argv)
{
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



    /// read temperature and pressure 10 times
    while (1) {
        float temperature = 0.0;

        /// temperature...
        if (bmp180.readTemperature(&temperature) < 0) {
            std::cerr << __func__ << ":" << __LINE__ << " readTemperature failed" << std::endl;
            return -1;
        }

        long pressure = 0.0;
        /// pressure...
        if (bmp180.readPressure(&pressure, oss_mode, 1, true) < 0) {
            std::cerr << __func__ << ":" << __LINE__ << " readPressure failed" << std::endl;
            return 0;
        }

        char buf[80];
        memset(buf, 0, 80);

        int size = logTimeString(buf);
        if (size < 0) {
            std::cerr << __func__ << ":" << __LINE__ << " logTimeString failed" << std::endl;
            return 0;
        }

        size += sprintf(buf+size, " %3.1f %ld\n", temperature, pressure);
        //printf("time string %s\n", buf);

        int fd = openFile();
        if (fd < 0) {
            std::cerr << __func__ << ":" << __LINE__ << " openFile failed" << std::endl;
            return 0;
        }

        if (write(fd, buf, size) != size) {
            std::cerr << __func__ << ":" << __LINE__ << " write failed" << std::endl;
            close(fd);
            return 0;
        }

        close(fd);

        /// sleep for one minute
        sleep(60);
    }

    if (bmp180.closeDevice() < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " closeDevice failed" << std::endl;
        return 0;
    }

    return 0;
}
