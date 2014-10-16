#include <iostream>
#include <iomanip>

#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>

#include <bmp180.h>
#include <binary.h>


/// ctor
Bmp180::Bmp180(char *device, unsigned char address) : Firmware_I2C(device, address) { }


int Bmp180::openDevice()
{
    /// openDevice in firmware_i2c open's the i2c device and set's the slave address (ioctl)
    if (Firmware_I2C::openDevice() < 0) {
        std::cerr << "Bmp180::" << __func__ << ":" << __LINE__ << " openDevice failed" << std::endl;
        return -1;
    }

    /// calibration start address
    unsigned char buffer[22] = { BMP180::EEPromStartAddress };

    /// write start address to device
    if (writeData(buffer, 1) < 0) {
        std::cerr << "Bmp180::" << __func__ << ":" << __LINE__ << " writeData failed" << std::endl;
        return -2;
    }

    /// read 22 bytes of calibration data
    if (readData(buffer, 22) < 0) {
        std::cerr << "Bmp180::" << __func__ << ":" << __LINE__ << " readData failed" << std::endl;
        return -3;
    }

    /// sanity check - no word should be 0xffff or 0x0000
    for (int i = 0; i < 22; i += 2) {

        /// shift in to 16 bit words and check
        short temp = ((buffer[i] & 0x00ff) << 8 | (buffer[i+1] & 0x00ff));
        if (temp == 0x0000 || temp == 0xffff) {
            std::cout << "Bmp180::" << __func__ << "(): Calibration error in byte " << i << " and " << i + 1 << std::endl;
            return -4;
        }
    }

    /// shift in to 16 bit words
    mCalibration.ac1 = buffer[0]  << 8 | buffer[1];
    mCalibration.ac2 = buffer[2]  << 8 | buffer[3];
    mCalibration.ac3 = buffer[4]  << 8 | buffer[5];
    mCalibration.ac4 = buffer[6]  << 8 | buffer[7];
    mCalibration.ac5 = buffer[8]  << 8 | buffer[9];
    mCalibration.ac6 = buffer[10] << 8 | buffer[11];
    mCalibration.b1  = buffer[12] << 8 | buffer[13];
    mCalibration.b2  = buffer[14] << 8 | buffer[15];
    mCalibration.mb  = buffer[16] << 8 | buffer[17];
    mCalibration.mc  = buffer[18] << 8 | buffer[19];
    mCalibration.md  = buffer[20] << 8 | buffer[21];

    if (mDebug) {
        std::cout << "ac1  0x" << std::hex << std::setw(4) << std::setfill('0') << mCalibration.ac1 << std::dec << std::endl;
        std::cout << "ac2  0x" << std::hex << std::setw(4) << std::setfill('0') << mCalibration.ac2 << std::dec << std::endl;
        std::cout << "ac3  0x" << std::hex << std::setw(4) << std::setfill('0') << mCalibration.ac3 << std::dec << std::endl;
        std::cout << "ac4  0x" << std::hex << std::setw(4) << std::setfill('0') << mCalibration.ac4 << std::dec << std::endl;
        std::cout << "ac5  0x" << std::hex << std::setw(4) << std::setfill('0') << mCalibration.ac3 << std::dec << std::endl;
        std::cout << "ac6  0x" << std::hex << std::setw(4) << std::setfill('0') << mCalibration.ac6 << std::dec << std::endl;
        std::cout << "b1   0x" << std::hex << std::setw(4) << std::setfill('0') << mCalibration.b1  << std::dec << std::endl;
        std::cout << "b2   0x" << std::hex << std::setw(4) << std::setfill('0') << mCalibration.b2  << std::dec << std::endl;
        std::cout << "mb   0x" << std::hex << std::setw(4) << std::setfill('0') << mCalibration.mb  << std::dec << std::endl;
        std::cout << "mc   0x" << std::hex << std::setw(4) << std::setfill('0') << mCalibration.mc  << std::dec << std::endl;
        std::cout << "md   0x" << std::hex << std::setw(4) << std::setfill('0') << mCalibration.md  << std::dec << std::endl;
        std::cout << std::dec;
    }

    return 0;
}


int Bmp180::readChipId()
{
    /// write chip ID chip-address
    unsigned char buffer[1] = { BMP180::ChipIdAddress };
    if (writeData(buffer, 1) < 0) {
        std::cerr << "Bmp180::" << __func__ << ":" << __LINE__ << " writeData failed" << std::endl;
        return -1;
    }

    /// read chip ID
    if (readData(buffer, 1) < 0) {
        std::cerr << "Bmp180::" << __func__ << ":" << __LINE__ << " readData failed" << std::endl;
        return -2;
    }

    /// return chip ID
    return buffer[0];
}


/** read temperature from the bmp180 pressure sensor
 *
 * @pascal: The measured temerature goes here
 *
 * @return: 0 on success, negative on failure
 */
int Bmp180::readTemperature(float *temperature)
{
    if (readTemperature() < 0) {
        std::cerr << "Bmp180::" << __func__ << ":" << __LINE__ << " readTemperature failed" << std::endl;
        return -1;
    }

    *temperature = ((mCalc.b5 + 8) >> 4) / 10.0;  /* temperature in deg C*/
    return 0;
}


int Bmp180::readTemperature()
{
    /// setup for temperature measurement
    unsigned char buffer[] = { BMP180::ControlRegisterAddress, BMP180::TemperatureConfig };

    if (writeData(buffer, 2) < 0) {
        std::cerr << "Bmp180::" << __func__ << ":" << __LINE__ << " writeData failed" << std::endl;
        return -1;
    }

    /// sleep for 4.5 ms
    usleep(4500);

    /// set read address
    if (writeData((unsigned char*)&BMP180::ConvertedValueMsb, 1) < 0) {
        std::cerr << "Bmp180::" << __func__ << ":" << __LINE__ << " writeData failed" << std::endl;
        return -2;
    }

    /// read temperature data
    if (readData(buffer, 2) < 0) {
        std::cerr << "Bmp180::" << __func__ << ":" << __LINE__ << " readData failed" << std::endl;
        return -3;
    }

    /// calculate temperature
    mCalc.UT = buffer[0] << 8 | buffer[1];

    mCalc.x1 = (((long)mCalc.UT - (long) mCalibration.ac6) * (long) mCalibration.ac5) >> 15;
    mCalc.x2 = ((long) mCalibration.mc << 11) / (mCalc.x1 + mCalibration.md);
    mCalc.b5 = mCalc.x1 + mCalc.x2;

    if (mDebug) {
        std::cout << "UT     " << std::dec << mCalc.UT << std::endl;
        std::cout << "x1     " << std::dec << mCalc.x1 << std::endl;
        std::cout << "x2     " << std::dec << mCalc.x2 << std::endl;
        std::cout << "b5     " << std::dec << mCalc.b5 << std::endl;
        std::cout << std::endl;
    }

    return 0;
}


/** read pressure from the bmp180 pressure sensor
 *
 * @pascal: converted pressure array
 * @oss: OverSamplingSetting (1, 2, 4 or 8 oversamples - delay: 4.5ms, 7.5ms, 13.5ms and 25.5ms)
 * @samples: numbers of samples to read (into pascal[])
 * @update_temperature: wheter to update temperature reading or not (say yes to this if current reading is older than 1 sec.)
 *
 * @return: 0 on success, negative on failure
 */
int Bmp180::readPressure(long pascal[], int oss, int samples, bool update_temperature)
{
    for (int i = 0; i < samples; i++) {
        //struct timeval  tv1, tv2;
        //gettimeofday(&tv1, NULL);
        if (readPressure(oss, update_temperature) < 0) {
            std::cerr << "Bmp180::" << __func__ << ":" << __LINE__ << " readPressure failed" << std::endl;
            return -1;
        }
        //gettimeofday(&tv2, NULL);
        //printf ("Total time = %f seconds\n", (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec));

        pascal[i] = mCalc.p;
    }

    return 0;
}


int Bmp180::readPressure(int oss, bool update_temperature)
{
    /// make sure requested oss is reasonable
    if (ModeOss0 < 0 || oss > ModeOss3) {
        std::cerr << "Bmp180::" << __func__ << ":" << __LINE__ << " invalid oss " << oss << std::endl;
        return -1;
    }

    /// read temperature if requested (If old temp measurement is older that 1 sec. do a new temp reading)
    if (update_temperature) {
        if (readTemperature() < 0) {
            std::cerr << "Bmp180::" << __func__ << ":" << __LINE__ << " readtemperature failed" << std::endl;
            return -2;
        }
    }

    /// need three bytes when reading back data - buffer[0] preinitialized with control register address
    unsigned char buffer[3] = { BMP180::ControlRegisterAddress, 0, 0 };

    /// Hardware Accuracy Modes including bit pattern for pressure read request
    switch (oss) {
    case ModeOss0: buffer[1] = BMP180::PressureOss0; break;
    case ModeOss1: buffer[1] = BMP180::PressureOss1; break;
    case ModeOss2: buffer[1] = BMP180::PressureOss2; break;
    case ModeOss3: buffer[1] = BMP180::PressureOss3; break;
    default: buffer[1] = BMP180::PressureOss0;
    }

    /// write control register address followed by control register conten choosen above (2 bytes)
    if (writeData(buffer, 2) < 0) {
        std::cerr << "Bmp180::" << __func__ << ":" << __LINE__ << " writeData failed" << std::endl;
        return -3;
    }

    /// sleep acording to oss mode
    switch (oss) {
    case ModeOss0: usleep(4500); break;
    case ModeOss1: usleep(7500); break;
    case ModeOss2: usleep(13500); break;
    case ModeOss3: usleep(25500); break;
    default: usleep(4500);
    }

    /// write address to measured value's msb byte
    memset(buffer, 0, 3);
    buffer[0] = BMP180::ConvertedValueMsb;
    if (writeData(buffer, 1) < 0) {
        std::cerr << "Bmp180::" << __func__ << ":" << __LINE__ << " writeData failed" << std::endl;
        return -4;
    }

    /// read measured pressure (3 bytes)
    if (readData(buffer, 3) < 0) {
        std::cerr << "Bmp180::" << __func__ << ":" << __LINE__ << " readData failed" << std::endl;
        return -5;
    }

    if (mDebug) {
        Binary binary;
        binary.printByteAsBinary("buffer 0", mBuffer[0]);
        binary.printByteAsBinary("buffer 1", mBuffer[1]);
        binary.printByteAsBinary("buffer 2", mBuffer[2]);
    }

    /// do the pressure calculation
    mCalc.UP = (((unsigned long) buffer[0] << 16) | ((unsigned long) buffer[1] << 8) | (unsigned long) buffer[2]) >> (8 - oss);
    if (mDebug) std::cout << "UT " << std::dec << mCalc.UT << std::endl;

    mCalc.b6 = mCalc.b5 - 4000;
    if (mDebug) std::cout << "b6 " << std::dec << mCalc.b6 << std::endl;

    mCalc.x1 = (mCalc.b6 * mCalc.b6) >> 12;
    mCalc.x1 *= mCalibration.b2;
    mCalc.x1 >>= 11;
    if (mDebug) std::cout << "x1 " << std::dec << mCalc.x1 << std::endl;

    mCalc.x2 = (mCalibration.ac2 * mCalc.b6);
    mCalc.x2 >>= 11;
    if (mDebug) std::cout << "x2 " << std::dec << mCalc.x2 << std::endl;

    mCalc.x3 = mCalc.x1 + mCalc.x2;
    if (mDebug) std::cout << "x3 " << std::dec << mCalc.x3 << std::endl;

    mCalc.b3 = (((((long)mCalibration.ac1) * 4 + mCalc.x3) << oss) + 2) >> 2;
    if (mDebug) std::cout << "b3 " << std::dec << mCalc.b3 << std::endl;

    mCalc.x1 = (mCalibration.ac3 * mCalc.b6) >> 13;
    if (mDebug) std::cout << "x1 " << std::dec << mCalc.x1 << std::endl;

    mCalc.x2 = (mCalibration.b1 * ((mCalc.b6 * mCalc.b6) >> 12)) >> 16;
    if (mDebug) std::cout << "x2 " << std::dec << mCalc.x2 << std::endl;

    mCalc.x3 = ((mCalc.x1 + mCalc.x2) + 2) >> 2;
    if (mDebug) std::cout << "x3 " << std::dec << mCalc.x3 << std::endl;

    mCalc.b4 = (mCalibration.ac4 * (unsigned long) (mCalc.x3 + 32768)) >> 15;
    if (mDebug) std::cout << "b4 " << std::dec << mCalc.b4 << std::endl;

    mCalc.b7 = ((unsigned long)(mCalc.UP - mCalc.b3) * (50000 >> oss));
    if (mDebug) std::cout << "b7 " << std::dec << mCalc.b7 << std::endl;

    if (mCalc.b7 < 0x80000000)
        mCalc.p = (mCalc.b7 << 1) / mCalc.b4;
    else
        mCalc.p = (mCalc.b7 / mCalc.b4) << 1;
    if (mDebug) std::cout << "p " << std::dec << mCalc.p << std::endl;

    mCalc.x1 = mCalc.p >> 8;
    mCalc.x1 *= mCalc.x1;
    mCalc.x1 = (mCalc.x1 * 3038) >> 16;
    if (mDebug) std::cout << "x1 " << std::dec << mCalc.x1 << std::endl;

    mCalc.x2 = (mCalc.p * -7357) >> 16;
    if (mDebug) std::cout << "x2 " << std::dec << mCalc.x2 << std::endl;

    /// pressure in Pa
    mCalc.p += (mCalc.x1 + mCalc.x2 + 3791) >> 4;
    if (mDebug) std::cout << "p " << std::dec << mCalc.p << std::endl;

    return 0;
}


/// NONE MEMBER FUNCTION

/// calculate altitude from give pressure at location and pressure at sea level
/// (international barometric formula)
double altitude(long pa, long p0)
{
    int Talkative = 0;

    if (Talkative) {
        std::cout << std::endl;
        std::cout << "p0        " << std::dec << p0 << std::endl;
        std::cout << "pa        " << std::dec << pa << std::endl;
        std::cout << std::endl;
    }

    double pp = (double)pa / (double)p0;
    if (Talkative) std::cout << "pp        " << std::dec << pp << std::endl;

    double ext = 1.0 / 5.255;
    if (Talkative) std::cout << "ext       " << std::dec << ext << std::endl;

    /// (pa / p0) ** (1 / 5.255)
    double ppowext = pow(pp, ext);
    if (Talkative) std::cout << "ppowext   " << std::dec << ppowext << std::endl;

    double altitude = (44330 * (1 - ppowext));
    if (Talkative) std::cout << "alititude " << std::dec << ppowext << std::endl;

    return altitude;
}


/// If you know your altitude then p0 (pressure at sea level)
/// can be calculated using the measured pressure.
double pressure_at_sea(long p, double altitude)
{
    return p / pow((1.0 - (altitude / 44330.0)), 5.255);
}
