#include <iostream>

#include <stdio.h>
#include <unistd.h>

#include <math.h>

#include <bmp180.h>
#include <binary.h>


Bmp180::Bmp180(char *device, unsigned char address) : Firmware_I2C(device, address) { }

int Bmp180::setResolution()
{

    return 0;
}

int Bmp180::initialize()
{
    mBuffer[0] = BMP180::EEPromStartAddress;        /// eeprom start address

    if (mDebug) {
        Binary binary;
        binary.printByteAsBinary("EEProm start address", mBuffer[0]);
    }

    int status = write(mFd, mBuffer, 1);
    if (status != 1) {
        std::cerr << "Bmp180::"<< __func__ << "(): write failed " << status << std::endl;
        return -2;
    }

    status = read(mFd, mBuffer, 22);
    if (status != 22) {
        std::cerr << "Bmp180::"<< __func__ << "(): read failed " << status << std::endl;
        return -3;
    }

    /// sanity check
    for (int i = 0; i < 22; i+=2) {
        short temp = mBuffer[i] << 8 | mBuffer[i+1];
        if (temp == 0x0000 || temp == 0xffff) {
            std::cout << "Bmp180::" << __func__ << "(): Calibration error in byte " << i << " and " << i + 1 << std::endl;
            return -4;
        }
    }

    mAc1 = mBuffer[0] << 8 | mBuffer[1];
    mAc2 = mBuffer[2] << 8 | mBuffer[3];
    mAc3 = mBuffer[4] << 8 | mBuffer[5];
    mAc4 = mBuffer[6] << 8 | mBuffer[7];
    mAc5 = mBuffer[8] << 8 | mBuffer[9];
    mAc6 = mBuffer[10] << 8 | mBuffer[11];

    mB1 = mBuffer[12] << 8 | mBuffer[13];
    mB1 = mBuffer[14] << 8 | mBuffer[15];

    mMb = mBuffer[16] << 8 | mBuffer[17];
    mMc = mBuffer[18] << 8 | mBuffer[19];
    mMd = mBuffer[20] << 8 | mBuffer[21];

    if (mDebug) {
        Binary binary;
        //for (int i = 0; i < 22; i++)
        //  binary.printByteAsBinary("buffer 0", mBuffer[i]);

        binary.printShortAsBinary("AC1", mAc1);
        binary.printShortAsBinary("AC2", mAc2);
        binary.printShortAsBinary("AC3", mAc3);
        binary.printShortAsBinary("AC4", mAc4);
        binary.printShortAsBinary("AC5", mAc5);
        binary.printShortAsBinary("AC6", mAc6);

        binary.printShortAsBinary("MB1", mB1);
        binary.printShortAsBinary("MB2", mB2);

        binary.printShortAsBinary("MB", mMb);
        binary.printShortAsBinary("MC", mMc);
        binary.printShortAsBinary("MD", mMd);
    }

    if (0) {
        printf("AC1 %d\n", mAc1);
        printf("AC2 %d\n", mAc2);
        printf("AC3 %d\n", mAc3);
        printf("AC4 %d\n", mAc4);
        printf("AC5 %d\n", mAc5);
        printf("AC6 %d\n", mAc6);

        printf("B1 %d\n", mB1);
        printf("B2 %d\n", mB2);

        printf("MB %d\n", mMb);
        printf("MC %d\n", mMc);
        printf("MD %d\n", mMd);
    }

    return 0;
}


int Bmp180::readTemperatur()
{
    mBuffer[0] = BMP180::ControlRegisterAddress;        /// register to be written
    mBuffer[1] = BMP180::TemperaturConfig;              /// value to be written to register

    if (mDebug){
        Binary binary;
        binary.printByteAsBinary("ControlRegisterAddress", mBuffer[0]);
        binary.printByteAsBinary("Temperatur config     ", mBuffer[1]);
    }

    int status = write(mFd, mBuffer, 2);
    if (status != 2) {
        std::cerr << "Bmp180::"<< __func__ << "(): write failed " << status << std::endl;
        return -1;
    }

    usleep(4500);

    mBuffer[0] = BMP180::ConvertedValueMsb;        /// read Convert Register (read 2 bytes: msb + lsb)

    if (mDebug) {
        Binary binary;
        binary.printByteAsBinary("Converted Value MSB register address", mBuffer[0]);
    }

    status = write(mFd, mBuffer, 1);
    if (status != 1) {
        std::cerr << "Bmp180::"<< __func__ << "(): write failed " << status << std::endl;
        return -2;
    }


    status = read(mFd, mBuffer, 2);
    if (status != 2) {
        std::cerr << "Bmp180::"<< __func__ << "(): read failed " << status << std::endl;
        return -3;
    }

    if (mDebug) {
        Binary binary;
        binary.printByteAsBinary("buffer 0", mBuffer[0]);
        binary.printByteAsBinary("buffer 1", mBuffer[1]);
    }

    mCalc.UT = mBuffer[0] << 8 | mBuffer[1];
    mCalc.x1 = (mCalc.UT - mAc6) * mAc5 / 32768;
    mCalc.x2 = mMc * 2048 / (mCalc.x1 + mMd);
    mCalc.b5 = mCalc.x1 + mCalc.x2;

    mTemperatur = (float)(mCalc.b5 + 8) / 16.0 / 10.0;

    if (mDebug) {
        printf("UT          %d\n", mCalc.UT);
        printf("X1          %d\n", mCalc. x1);
        printf("X2          %d\n", mCalc.x2);
        printf("B5          %d\n", mCalc.b5);
        printf("Temperatur  %4.2f celcius\n");
        printf("\n");
    }

    if (mDebug) printf("Temeratur %3.1f\n", mTemperatur);

    return 0;
}


long Bmp180::readPressure(int oss, bool readtemp)
{
    /// make sure requested oss is reasonable
    if (ModeOss0 < 0 || oss > ModeOss3)
        oss = ModeOss0;

    /// read temperatur if requested
    if (readtemp) {
        if (readTemperatur() < 0) {
            std::cerr << "Bmp180::" << __func__ << "(): readTemperatur failed" << std::endl;
            return -1;
        }
    }

    /// register to be written (ControlRegister)
    mBuffer[0] = BMP180::ControlRegisterAddress;

    /// set second byte acording to oss
    switch (oss) {
    case ModeOss0: mBuffer[1] = BMP180::PressureOss0; break;
    case ModeOss1: mBuffer[1] = BMP180::PressureOss1; break;
    case ModeOss2: mBuffer[1] = BMP180::PressureOss2; break;
    case ModeOss3: mBuffer[1] = BMP180::PressureOss3; break;
    default: mBuffer[1] = BMP180::PressureOss0;
    }

    if (mDebug) {
        Binary binary;
        binary.printByteAsBinary("ControlRegisterAddress", mBuffer[0]);
        binary.printByteAsBinary("Pressure oss 0 config ", mBuffer[1]);
    }

    /// write PressureOss_x requested to control register
    int status = write(mFd, mBuffer, 2);
    if (status != 2) {
        std::cerr << "Bmp180::"<< __func__ << "(): write failed " << status << std::endl;
        return -1;
    }

    /// sleep acording to oss mode
    switch (oss) {
    case ModeOss0: usleep(4500); break;
    case ModeOss1: usleep(7500); break;
    case ModeOss2: usleep(13500); break;
    case ModeOss3: usleep(25500); break;
    default: usleep(4500);
    }

    /// this is the register address to the converted results (msb at this address, lsb on the next)
    mBuffer[0] = BMP180::ConvertedValueMsb;

    if (mDebug) {
        Binary binary;
        binary.printByteAsBinary("Converted Value MSB register address", mBuffer[0]);
    }

    /// write address
    status = write(mFd, mBuffer, 1);
    if (status != 1) {
        std::cerr << "Bmp180::"<< __func__ << "(): write failed " << status << std::endl;
        return -2;
    }


    /// read result
    //status = read(mFd, mBuffer, 3);
    //if (status != 3) {
    status = read(mFd, mBuffer, 2);
    if (status != 2) {
        std::cerr << "Bmp180::"<< __func__ << "(): read failed " << status << std::endl;
        return -3;
    }

    if (mDebug) {
        Binary binary;
        binary.printByteAsBinary("buffer 0", mBuffer[0]);
        binary.printByteAsBinary("buffer 1", mBuffer[1]);
        binary.printByteAsBinary("buffer 2", mBuffer[2]);
    }

    /// do the pressure calculation
    //mCalc.UP = (mBuffer[0] << 16) | (mBuffer[1] << 8) | (mBuffer[2] >> (8 - oss));
    mCalc.UP = (mBuffer[0] << 8) | (mBuffer[1]);
    if (mDebug) printf("UP %d\n", mCalc.UP);

    mCalc.b6 = mCalc.b5 - 4000;
    if (mDebug) printf("B6 %d\n", mCalc.b6);

    mCalc.x1 = (mB2 * (mCalc.b6 * mCalc.b6 / 4096)) / 2048;
    if (mDebug) printf("X1 %d\n", mCalc.x1);

    mCalc.x2 = mAc2 * mCalc.b6 / 2048;
    if (mDebug) printf("X2 %d\n", mCalc.x2);

    mCalc.x3 = mCalc.x1 + mCalc.x2;
    if (mDebug) printf("X3 %d\n", mCalc.x3);

    mCalc.b3 = (((mAc1 * 4 + mCalc.x3) << oss) + 2) / 4;
    if (mDebug) printf("B3 %d\n", mCalc.b3);

    mCalc.x1 = mAc3 * mCalc.b6 / 8192;
    if (mDebug) printf("X1 %d\n", mCalc.x1);

    mCalc.x2 = (mB1 * (mCalc.b6 * mCalc.b6 / 4096)) / 65536;
    if (mDebug) printf("X2 %d\n", mCalc.x2);

    mCalc.x3 = (( mCalc.x1 + mCalc.x2) + 2) / 4;
    if (mDebug) printf("X3 %d\n", mCalc.x3);

    mCalc.b4 = mAc4 * (unsigned long)(mCalc.x3 + 32768) / 32768;
    if (mDebug) printf("B4 %ul\n", mCalc.b4);

    mCalc.b7 = ((unsigned long)mCalc.UP - mCalc.b3) * (50000 >> oss);
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
    if (mDebug) printf("P  %d\n", mCalc.p);

    /// back up pressure
    mPressure = mCalc.p;

    /// return the pressure
    return mCalc.p;
}


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
