#include <iostream>
#include <iomanip>

#include <stdio.h>
#include <unistd.h>

#include <math.h>

#include <bmp180.h>
#include <binary.h>


/// ctor
Bmp180::Bmp180(char *device, unsigned char address) : Firmware_I2C(device, address) { }


int Bmp180::openDevice()
{
    /// openDevice in firmware_i2c open's the i2c device and set's the slave address (ioctl)
    if (Firmware_I2C::openDevice() < 0)
        return -1;

    unsigned char buffer[22] = { BMP180::EEPromStartAddress };

    /// write start address
    if (writeData(buffer, 1) < 0)
        return -2;

    /// read 22 bytes of calibration data
    if (readData(buffer, 22) < 0)
        return -3;

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
    mCalibration.mAc1 = buffer[0]  << 8 | buffer[1];
    mCalibration.mAc2 = buffer[2]  << 8 | buffer[3];
    mCalibration.mAc3 = buffer[4]  << 8 | buffer[5];
    mCalibration.mAc4 = buffer[6]  << 8 | buffer[7];
    mCalibration.mAc5 = buffer[8]  << 8 | buffer[9];
    mCalibration.mAc6 = buffer[10] << 8 | buffer[11];
    mCalibration.mB1  = buffer[12] << 8 | buffer[13];
    mCalibration.mB2  = buffer[14] << 8 | buffer[15];
    mCalibration.mMb  = buffer[16] << 8 | buffer[17];
    mCalibration.mMc  = buffer[18] << 8 | buffer[19];
    mCalibration.mMd  = buffer[20] << 8 | buffer[21];

    if (mDebug) {
        std::cout << "AC1  0x" << std::hex << std::setw(4) << std::setfill('0') << mCalibration.mAc1 << std::endl;
        std::cout << "AC2  0x" << std::hex << std::setw(4) << std::setfill('0') << mCalibration.mAc2 << std::endl;
        std::cout << "AC3  0x" << std::hex << std::setw(4) << std::setfill('0') << mCalibration.mAc3 << std::endl;
        std::cout << "AC4  0x" << std::hex << std::setw(4) << std::setfill('0') << mCalibration.mAc4 << std::endl;
        std::cout << "AC5  0x" << std::hex << std::setw(4) << std::setfill('0') << mCalibration.mAc3 << std::endl;
        std::cout << "AC6  0x" << std::hex << std::setw(4) << std::setfill('0') << mCalibration.mAc6 << std::endl;
        std::cout << "B1   0x" << std::hex << std::setw(4) << std::setfill('0') << mCalibration.mB1 << std::endl;
        std::cout << "B2   0x" << std::hex << std::setw(4) << std::setfill('0') << mCalibration.mB2 << std::endl;
        std::cout << "MB   0x" << std::hex << std::setw(4) << std::setfill('0') << mCalibration.mMb << std::endl;
        std::cout << "MC   0x" << std::hex << std::setw(4) << std::setfill('0') << mCalibration.mMc << std::endl;
        std::cout << "MD   0x" << std::hex << std::setw(4) << std::setfill('0') << mCalibration.mMd << std::endl;
    }

    return 0;
}

int Bmp180::readChipId()
{
    /// write chip ID chip-address
    unsigned char buffer[1] = { BMP180::ChipIdAddress };
    if (writeData(buffer, 1) < 0)
        return -1;

    /// read chip ID
    if (readData(buffer, 1) < 0)
        return -2;

    /// return chip ID
    return buffer[0];
}


int Bmp180::readTemperatur(float *temperatur)
{
    if (readTemperatur() < 0)
        return -1;

    *temperatur = (float)(mCalc.b5 + 8) / 16.0 / 10.0;

    return 0;
}


int Bmp180::readTemperatur()
{
    /// setup for temperatur measurement
    unsigned char buffer[] = { BMP180::ControlRegisterAddress, BMP180::TemperaturConfig };
    if (writeData(buffer, 2) < 0)
        return -1;

    /// sleep for 4.5 ms
    usleep(4500);

    /// set read address
    if (writeData((unsigned char*)&BMP180::ConvertedValueMsb, 1) < 0)
        return -2;

    /// read temperatur data
    if (readData(buffer, 2) < 0)
        return -3;

    /// calculate temperatur
    mCalc.UT = buffer[0] << 8 | buffer[1];
    mCalc.x1 = (mCalc.UT - mCalibration.mAc6) * mCalibration.mAc5 / 32768;
    mCalc.x2 = mCalibration.mMc * 2048 / (mCalc.x1 + mCalibration.mMd);
    mCalc.b5 = mCalc.x1 + mCalc.x2;

    if (mDebug) {
        printf("UT          %d\n", mCalc.UT);
        printf("X1          %d\n", mCalc.x1);
        printf("X2          %d\n", mCalc.x2);
        printf("B5          %d\n", mCalc.b5);
        printf("\n");
    }

    return 0;
}


int Bmp180::readPressure(long *pascal, int oss, bool update_temperatur)
{
    if (readPressure(oss, update_temperatur) < 0)
        return -1;

    *pascal = mCalc.p;

    return 0;
}


int Bmp180::readPressure(int oss, bool update_temperatur)
{
    /// make sure requested oss is reasonable
    if (ModeOss0 < 0 || oss > ModeOss3) {
        std::cerr << "Bmp180::" << __func__ << "(): invalid oss " << oss << std::endl;
        return -1;
    }

    /// read temperatur if requested (If old temp measurement is older that 1 sec. do a new temp reading)
    if (update_temperatur) {
        if (readTemperatur() < 0) {
            std::cerr << "Bmp180::" << __func__ << "(): readTemperatur failed" << std::endl;
            return -2;
        }
    }

    /// need three bytes when reading back data
    unsigned char buffer[3] = { BMP180::ControlRegisterAddress };

    /// Hardware Accuracy Modes (see data sheet for details)
    switch (oss) {
    case ModeOss0: buffer[1] = BMP180::PressureOss0; break;
    case ModeOss1: buffer[1] = BMP180::PressureOss1; break;
    case ModeOss2: buffer[1] = BMP180::PressureOss2; break;
    case ModeOss3: buffer[1] = BMP180::PressureOss3; break;
    default: buffer[1] = BMP180::PressureOss0;
    }

    if (writeData(buffer, 2) < 0)
        return -1;

    /// sleep acording to oss mode
    switch (oss) {
    case ModeOss0: usleep(4500); break;
    case ModeOss1: usleep(7500); break;
    case ModeOss2: usleep(13500); break;
    case ModeOss3: usleep(25500); break;
    default: usleep(4500);
    }

    buffer[0] = BMP180::ConvertedValueMsb;
    if (readData(buffer, 1) < 0)
        return -2;

    if (readData(buffer, 3) < 0)
        return -3;

    if (mDebug) {
        Binary binary;
        binary.printByteAsBinary("buffer 0", mBuffer[0]);
        binary.printByteAsBinary("buffer 1", mBuffer[1]);
        binary.printByteAsBinary("buffer 2", mBuffer[2]);
    }

    /// do the pressure calculation
    mCalc.UP = (unsigned long)(buffer[0] << 16) | (unsigned long)(buffer[1] << 8) | (unsigned long)(buffer[2] >> (8 - oss));
    if (mDebug) printf("UP %d\n", mCalc.UP);

    mCalc.b6 = mCalc.b5 - 4000;
    if (mDebug) printf("B6 %d\n", mCalc.b6);

    mCalc.x1 = (mCalibration.mB2 * (mCalc.b6 * mCalc.b6 / 4096)) / 2048;
    if (mDebug) printf("X1 %d\n", mCalc.x1);

    mCalc.x2 = mCalibration.mAc2 * mCalc.b6 / 2048;
    if (mDebug) printf("X2 %d\n", mCalc.x2);

    mCalc.x3 = mCalc.x1 + mCalc.x2;
    if (mDebug) printf("X3 %d\n", mCalc.x3);

    //mCalc.b3 = ((((long)mAc1 * 4 + mCalc.x3) << oss) + 2) / 4;
    mCalc.b3 = (((((long)mCalibration.mAc1) * 4 + mCalc.x3) << oss) + 2) >> 2;
    if (mDebug) printf("B3 %d\n", mCalc.b3);

    mCalc.x1 = (mCalibration.mAc3 * mCalc.b6) >> 13;
    //mCalc.x1 = mAc3 * mCalc.b6 / 8192;
    if (mDebug) printf("X1 %d\n", mCalc.x1);

    mCalc.x2 = (mCalibration.mB1 * ((mCalc.b6 * mCalc.b6) >> 12)) >> 16;
    //mCalc.x2 = (mB1 * (mCalc.b6 * mCalc.b6 / 4096)) / 65536;
    if (mDebug) printf("X2 %d\n", mCalc.x2);

    mCalc.x3 = (( mCalc.x1 + mCalc.x2) + 2) / 4;
    if (mDebug) printf("X3 %d\n", mCalc.x3);

    mCalc.b4 = mCalibration.mAc4 * (unsigned long)(mCalc.x3 + 32768) / 32768;
    if (mDebug) printf("B4 %ul\n", mCalc.b4);

    mCalc.b7 = ((unsigned long)(mCalc.UP - mCalc.b3) * (50000 >> oss));
    //mCalc.b7 = ((unsigned long)mCalc.UP - mCalc.b3) * (50000 >> oss);
    if (mDebug) printf("B7 %ul\n", mCalc.b7);

    if (mCalc.b7 < 0x80000000)
        mCalc.p = (mCalc.b7 * 2) / mCalc.b4;
    else
        mCalc.p = (mCalc.b7 / mCalc.b4) * 2;
    if (mDebug) printf("P  %d\n", mCalc.p);

    mCalc.x1 = (mCalc.p / 256) * (mCalc.p / 256);
    if (mDebug) printf("X1 %d\n", mCalc.x1);

    mCalc.x1 = (mCalc.x1 * 3038) / 65536;
    if (mDebug) printf("X1 %d\n", mCalc.x1);

    mCalc.x2 = (-7357 * mCalc.p) / 65536;
    if (mDebug) printf("X2 %d\n", mCalc.x2);

    mCalc.p = mCalc.p + (mCalc.x1 + mCalc.x2 + 3791) / 16;
    if (mDebug) printf("Pressure  %d Pa\n", mCalc.p);

    /// return the pressure
    return 0;
}

/// NONE MEMBER FUNCTION

/// calculate altitude from give pressure at location and pressure at sea level
/// (international barometric formula)
double altitude(long pa, long p0)
{
    int Talkative = 0;

    if (Talkative) {
        printf("\n");
        printf("p0        %df\n", p0);
        printf("pa        %d\n", pa);
    }

    double pp = (double)pa / (double)p0;
    if (Talkative) printf("pp        %4.6f\n", pp);

    double ext = 1.0 / 5.255;
    if (Talkative) printf("ext       %4.6f\n", ext);

    /// (pa / p0) ** (1 / 5.255)
    double ppowext = pow(pp, ext);
    if (Talkative) printf("ppowext   %4.6f\n", ppowext);

    double altitude = (44330 * (1 - ppowext));
    if (Talkative) printf("alititude %4.2f\n\n", altitude);

    return altitude;
}
