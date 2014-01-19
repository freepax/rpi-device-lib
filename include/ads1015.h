#ifndef ADS1015_H
#define ADS1015_H

static const int Ads1015Address0 = 0b01001000;
static const int Ads1015Address1 = 0b01001001;

#include <firmware_i2c.h>


/**
 * Pointer register - one byte
 * ============================
 *
 * [ x x x x x x B1 B0 ]
 *
 * B1 B0
 *  0  0 : Conversion regiser
 *  0  1 : Config register
 *  1  0 : Lo thresh register
 *  1  1 : Hi thresh register
 *
 *
 * Config register - two bytes
 * ============================
 *
 * MSB [ OS MUX2 MUX1 MUX0 PGA2 PGA1 PGA0 MODE ]
 * LSB [ DR2 DR1 DR0 COMP_MODE COMP_POL COMP_LAT COMP_QUE1 COMP_QUE0 ]
 *
 * Bit    15: OS: Operational status/single-shot conversion start (write: 0 No effect)
 * Bit 14:12: MUX[2:0]: Input multiplexer configuration (ADS1015 only) (default 000 : AIN P = AIN0 and AIN N = AIN1
 * Bit 11: 9: PGA[2:0]: Programmable gain amplifier configuration (ADS1014 and ADS1015 only) (default 010 ±2.048V)
 * Bit     8: MODE: Device operating mode (default 1 Power-down single-shot mode)
 * Bit  7: 5: DR[2:0]: Data rate (default 100 1600SPS)
 * Bit     4: COMP_MODE: Comparator mode (ADS1014 and ADS1015 only) (default 0)
 * Bit     3: COMP_POL: Comparator polarity (ADS1014 and ADS1015 only) (default 0)
 * Bit     2: COMP_LAT: Latching comparator (ADS1014 and ADS1015 only) (default 0)
 * Bit  1: 0: COMP_QUE: Comparator queue and disable (ADS1014 and ADS1015 only) (default 11 Disable)
 *
 *
 * Converstion register - two bytes
 * =================================
 *
 * MSB [ D11 D10 D9 D8 D7 D6 D5 D4 ]
 * LSB [ D3  D2  D1 D0  x  x  x  x ]
 *
 */


static const int OSNoEffectOnWrite = 0b0;
static const int OSStartConversion = 0b1;
static const int OSBusy            = 0b0;
static const int OSIdle            = 0b1;

static const int MUXAIN0AIN1 = 0b000;
static const int MUXAIN0AIN3 = 0b001;
static const int MUXAIN1AIN3 = 0b010;
static const int MUXAIN2AIN3 = 0b011;

static const int MUXAIN0GND  = 0b100;
static const int MUXAIN1GND  = 0b101;
static const int MUXAIN2GND  = 0b110;
static const int MUXAIN3GND  = 0b111;

static const int PGA6144   = 0b000;
static const int PGA4096   = 0b001;
static const int PGA2048   = 0b010;
static const int PGA1024   = 0b011;
static const int PGA0512   = 0b100;
static const int PGA0256_1 = 0b101;
static const int PGA0256_2 = 0b110;
static const int PGA0512_3 = 0b111;

static const int MODEContinous = 0b0;
static const int MODEPowerDown = 0b1;

static const int DR128    = 0b000;
static const int DR250    = 0b001;
static const int DR490    = 0b010;
static const int DR920    = 0b011;
static const int DR1600   = 0b100;
static const int DR2400   = 0b101;
static const int DR3300_1 = 0b110;
static const int DR3300_2 = 0b111;

static const int COMPMODEHysteresis = 0b0;
static const int COMPMODEWindow     = 0b1;

static const int COMPPOLActiveLow  = 0b0;
static const int COMPPOLActiveHigh = 0b1;

static const int COMPLATNoneLatching = 0b0;
static const int COMPLATLatching     = 0b1;

static const int COMPQUEAssertAfterOne  = 0b00;
static const int COMPQUEAssertAfterTwo  = 0b01;
static const int COMPQUEAssertAfterFour = 0b10;
static const int COMPQUEDisable         = 0b11;

static const int PointerConversionRegister = 0b00000000;
static const int PointerConfigRegister     = 0b00000001;
static const int PointerLoTresholdRegister = 0b00000010;
static const int PointerHiTresholdRegister = 0b00000011;

static const float ConversionOutOfRange = -999999;


class ADS1015 : public Firmware_I2C {
public:
    /// ctor
    ADS1015(char *device = (char*)FirmwareI2CDeviceses::i2c_0, int address = Ads1015Address0, bool debug = false);

    /// dtor
    ~ADS1015();

    /// write configuration to chip
    int writeConfig();

    /// initiate read (set pointer to conversion register)
    int initRead();

    /// read the device
    float readDevice();

    /// last value read
    float voltage() { return mVoltage; }

    /// set config msb
    int setOs(unsigned int);
    int setMux(unsigned int);
    int setPga(unsigned int);
    int setMode(unsigned int);

    /// set config lsb
    int setDr(unsigned int);
    int setCompMode(unsigned int);
    int setCompPol(unsigned int);
    int setCompLat(unsigned int);
    int setCompQue(unsigned int);

    /// get config msb
    unsigned int os() { return mOs; }
    unsigned int mux() { return mMux; }
    unsigned int pga() { return mPga; }
    unsigned int mode() { return mMode; }

    /// get config lsb
    unsigned int dr() { return mDr; }
    unsigned int compMode() { return mCompMode; }
    unsigned int compPol() { return mCompPol; }
    unsigned int compLat() { return mCompLat; }
    unsigned int compQue() { return mCompQue; }


private:
    /// config register msb
    unsigned int mOs;
    unsigned int mMux;
    unsigned int mPga;
    unsigned int mMode;

    /// config register lsb
    unsigned int mDr;
    unsigned int mCompMode;
    unsigned int mCompPol;
    unsigned int mCompLat;
    unsigned int mCompQue;

    /// Voltage reference, changed with pga
    float mVref;

    /// float holding the last value
    float mVoltage;
};

#endif /// ADS1015_H
