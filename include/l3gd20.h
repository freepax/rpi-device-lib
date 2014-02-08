#ifndef L3GD20_H
#define L3GD20_H

/// linux i2c driver headers
#include <firmware_i2c.h>


/**
  * L3GD20 Registers:
  *
  * WHO_AM_I:               Device identification register.
  *
  * Control Register 1:     [ DR1 DR0 BW1 BW0 PD Zen Xen Yen ]
  *   DR1 and DR0           Output data rate selection
  *   BW1 and BW0           Bandwidth selection
  *   PD                    Power down mode enable (0: disabled, 1: normal mode or sleep mode - default: 0)
  *   Zen                   Z axis enable (0: disabled, 1:enabled - default: 1)
  *   Yen                   Y axis enable (0: disabled, 1:enabled - default: 1)
  *   Xen                   X axis enable (0: disabled, 1:enabled - default: 1)
  *
  * Control Register 2:     [ 0 0 HPM1 HPM0 HPC3 HPC2 HPC1 HPC0 ]
  *   HPM1 and HPM0         Hi pass filter mode selection (default: 0)
  *   HPC3 through HPC0     Hi pass filter cut-off frequency selection
  *
  * Control Register 3:     [ I1_INT1 I1_BOOT H_LACTIVE PP_OD I2_DRDY I2_WTM I2_ORUN I2_EMPTY ]
  *   I1_INT1               Interrupt enable on INT1 pin (default: 0, 0: disable, 1: enable)
  *   I1_BOOT               Boot status available on INT1. (default: 0, 0: disable, 1: enable)
  *   H_LACTIVE             Interrupt active configuration on INT1 (default: 0 0: high, 1: low)
  *   PP_OD                 Push pull / Open drain (default: 0, 0: push-pull, 1: open drain)
  *   I2_DRDY               Data ready on DRDY/INT2. (default: 0, 0: disable, 1: enable)
  *   I2_WTM                FIFO watermark interrupt on DRDY/INT2 (default: 0, 0: disable, 1: enable)
  *   I2_ORUN               FIFO overrun interrupt on DRDY/INT2 (default: 0, 0: disable, 1: enable)
  *   I2_EMPTY              FIFO empty interrupt on DRDY/INT2 (default: 0, 0: disable, 1: enable)
  *
  * Control Register 4:     [ BDU BLE FS1 FS0 - 0 0 SIM ]
  *   BDU                   Block data update (default: 0, 0: continous update, 1: output registers not updated until MSb and LSb reading
  *   BLE                   Big/Little ending selection (default: 0, 0: data lsb @  lower address, 1: data msb @ lower address)
  *   FS1 and FS0           Full scale selection (default: 0 0, 00: 250 dps, 01: 500 dps, 10: 2000 dps, 11: 2000 dps)
  *   SIM                   SPI serial mode selection: (default: 0, 0: 4-wire interface, 1: 3-wire interface)
  *
  * Control Register 5:     [ BOOT FIFO_EN - HPen INT1_SEL1 INT1_SEL0 OUT_SEL1 OUT_SEL0 ]
  *   BOOT                  Reboot memory content (default: 0, 0: normal mode, 1: reboot memory content)
  *   FIFO_EN               FIFO enable (default: 0, 0: FIFO disable, 1: FIFO enable)
  *   HPen                  Hi pass filter enable (default: 0, 0: disable, 1: enable)
  *   INT1_SEL1 - INT1_SEL0 Interrupt selection (default: 0 0)
  *   OUT_SEL1 - OUT_SEL0   Output selection configuration (default: 0 0)
  *
  * Reference:              [ REF7 REF6 REF5 REF4 REF3 REF2 REF1 REF0 ]
  *   REF7 through REF0     Reference value for interrupt generation
  *
  * Temperatur:             [ TEMP7 TEMP6 TEMP5 TEMP4 TEMP3 TEMP2 TEMP1 TEMP0 ]
  *   TEMP7 - TEMP0         Temperature data
  *
  * Status Register:        [ ZYXOR ZOR YOR XOR ZYXDA ZDA YDA XDA ]
  *   ZYXOR                 x, y, z axis data overrun. Default 0, 0: no overrun, 1: new data has overwritten old before it was read
  *   ZOR                   Z data overrun. Default 0: 0: no overrun, 1: new z data has overwritten...
  *   YOR                   Y data overrun. Default 0: 0: no overrun, 1: new z data has overwritten...
  *   XOR                   X data overrun. Default 0: 0: no overrun, 1: new z data has overwritten...
  *   ZYXDA                 z, y, x axis new data available: default 0, 0: no new data, 1: new data available
  *   ZDA                   New Z axis data available. default 0, 0: no new z data available, 1: new z data available
  *   YDA                   New Y axis data available. default 0, 0: no new y data available, 1: new y data available
  *   XDA                   New X axis data available. default 0, 0: no new x data available, 1: new x data available
  *
  * Out XL and Out XH Register:
  *   X axis angular rate data: The value is expressed as two's complement
  *
  * Out YL and Out YH Register:
  *   Y axis angular rate data: The value is expressed as two's complement
  *
  * Out ZL and Out ZH Register:
  *   Z axis angular rate data: The value is expressed as two's complement
  *
  * FIFO Control Register:  [ FM2 FM1 FM0 WTM4 WTM3 WTM2 WTM1 WTM0 ]
  *   FM2 - FM0             Fifo mode selection default 0 0
  *   WTM4 - WTM0           Fifo threshold. Watermark level setting
  *
  * FIFO Source Register:   [ WTM OVRN EMPTY FSS4 FSS3 FSS2 FSS1 FSS0 ]
  *   WTM                   Watermark status: 0: Fifo filling is lower than level, 1: filling is equal or higher.
  *   OVRN                  Overrun bit status. 0: FIFO is not completely filled, 1: FIFO is filled
  *   EMPTY                 FIFO empty bit: 0: FIFO is not empty, 1: FIFO is empty
  *   FSS4 - FSS0           FIFO stored data level.
  *
  * INT1 Config Register:   [ AND/OR LIR ZHIE ZLIE YHIE YLIE XHIE XLIE ]
  *   AND/OR                And/Or combination of interrupt events. Default 0. 0: OR combination, 1: AND combination
  *   LIR                   Latch interrupt request. Default 0. 0: interrupt request not latched, 1: interrupt request latched.
  *   ZHIE                  Enabled interrupt generation on Z high event. Default 0. 0: disable, 1: enable interrupt on value higher than preset threshold
  *   ZLIE                  Enabled interrupt generation on Z lo event. Default 0. 0: disable, 1: enable interrupt on value lower than preset threshold
  *   YHIE                  Enabled interrupt generation on Y high event. Default 0. 0: disable, 1: enable interrupt on value higher than preset threshold
  *   YLIE                  Enabled interrupt generation on Y lo event. Default 0. 0: disable, 1: enable interrupt on value lower than preset threshold
  *   XHIE                  Enabled interrupt generation on X high event. Default 0. 0: disable, 1: enable interrupt on value higher than preset threshold
  *   XLIE                  Enabled interrupt generation on X lo event. Default 0. 0: disable, 1: enable interrupt on value lower than preset threshold
  *
  * INT1 source Register:   [ 0 IA ZH ZL YH YL XY XL ]
  *   IA                    Interrupt active. Default 0. 0: no interrupt have been generated. 1: one or more interrupt have been generated.
  *   ZH                    Z high. Default 0. 0: no interrupts. 1: Z high event has occured
  *   ZL                    Z low. Default 0. 0: no interrupts. 1: Z low event has occured
  *   YH                    Y high. Default 0. 0: no interrupts. 1: Y high event has occured
  *   YL                    Y low. Default 0. 0: no interrupts. 1: Y low event has occured
  *   XH                    X high. Default 0. 0: no interrupts. 1: X high event has occured
  *   XL                    X low. Default 0. 0: no interrupts. 1: X low event has occured
  *
  * INT1 X treshold High:   [ - THSX14 THSX13 THSX12 THSX10 THSX9 THSX8 ]
  *   THSX14 - THSX8        7 msb of interrupt threshold
  *
  * INT1 X treshold Low:    [ THSX7 THSX6 THSX5 THSX4 THSX3 THSX2 THSX1 THSX0 ]
  *   THSX14 - THSX8        8 lsb of interrupt threshold  *
  *
  * INT1 Y treshold High:   [ - THSY14 THSY13 THSY12 THSY10 THSY9 THSY8 ]
  *   THSY14 - THSY8        7 msb of interrupt threshold
  *
  * INT1 Y treshold Low:    [ THSY7 THSY6 THSY5 THSY4 THSY3 THSY2 THSY1 THSY0 ]
  *   THSY14 - THSY8        8 lsb of interrupt threshold  *
  *
  * INT1 Z treshold High:   [ - THSZ14 THSZ13 THSZ12 THSZ10 THSZ9 THSZ8 ]
  *   THSZ14 - THSZ8        7 msb of interrupt threshold
  *
  * INT1 Z treshold Low:    [ THSZ7 THSZ6 THSZ5 THSZ4 THSZ3 THSZ2 THSZ1 THSZ0 ]
  *   THSZ14 - THSZ8        8 lsb of interrupt threshold  *
  *
  * INT1 Duration           [ WAIT D6 D5 D4 D3 D2 D1 D0 ]
  *   WAIT                  WAIT enabled. Default 0. 0: disable. 1: enable
  *   D6 - D0               Duration value. Default 000 0000
  *
  */



/// two possible i2c addresses for L3GD20
namespace L3dg20 {
static const int L3gd20Address0 = 0b01101010;
static const int L3gd20Address1 = 0b01101011;
}


/// the L3GD20 register addresses
namespace L3dg20Register {
static const int WhoAddress                     = 0b00001111;
static const int ControlOneAddress              = 0b00100000;
static const int ControlTwoAddress              = 0b00100001;
static const int ControlThreeAddress            = 0b00100010;
static const int ControlFourAddress             = 0b00100011;
static const int ControlFiveAddress             = 0b00100100;
static const int ReferenceAddress               = 0b00100101;
static const int TemperaturAddress              = 0b00100110;
static const int StatusAddress                  = 0b00100111;
static const int OutXLAddress                   = 0b00101000;
static const int OutXHAddress                   = 0b00101001;
static const int OutYLAddress                   = 0b00101010;
static const int OutYHAddress                   = 0b00101011;
static const int OutZLAddress                   = 0b00101100;
static const int OutZHAddress                   = 0b00101101;
static const int FifoControlAddress             = 0b00101110;
static const int FifoControlSourceAddress       = 0b00101111;
static const int InteruptConfigureAddress       = 0b00110000;
static const int InteruptConfigureSourceAddress = 0b00110001;
static const int InteruptXHAddress              = 0b00110010;
static const int InteruptXLAddress              = 0b00110011;
static const int InteruptYHAddress              = 0b00110100;
static const int InteruptYLAddress              = 0b00110101;
static const int InteruptzHAddress              = 0b00110110;
static const int InteruptzlAddress              = 0b00110111;
static const int InteruptDurationAddress        = 0b00111000;
}


/// the L3GD20 register defaults
namespace L3dg20Defaults {
static const int Who                     = 0b11010100;
static const int ControlOne              = 0b00001111;
static const int ControlTwo              = 0b00000000;
static const int ControlThree            = 0b00000000;
static const int ControlFour             = 0b00000000;
static const int ControlFive             = 0b00000000;
static const int Reference               = 0b00000000;
static const int FifoControl             = 0b00000000;
static const int InteruptConfigure       = 0b00000000;
static const int InteruptXH              = 0b00000000;
static const int InteruptXL              = 0b00000000;
static const int InteruptYH              = 0b00000000;
static const int InteruptYL              = 0b00000000;
static const int InteruptzH              = 0b00000000;
static const int Interuptzl              = 0b00000000;
static const int InteruptDuration        = 0b00000000;
}

class L3GD20 : public Firmware_I2C {
public:
    /// ctor
    L3GD20(char *device = (char*)FirmwareI2CDeviceses::i2c_0, int address = L3dg20::L3gd20Address0, bool debug = false);

    /// dtor
    ~L3GD20();

    /// set and get i2c address
    int setAddress(int);

    /// Who Am I
    int whoAmI(unsigned char *serialnumber);

    /// read temperatur
    int readTemperatur(unsigned char *temperatur);

    /// read Z rotation (hi and lo byte)
    int readZrotation(short *zrotation);
    //int readZrotation(int *zrotation);

    int writeRegister(unsigned char address, unsigned char byte);
    //int setControlOne(unsigned char dr, unsigned char bw, bool pd, bool zen, bool xen, bool yen);
    int setControlOne();
    int setControlTwo(unsigned char hpm, unsigned char hpc);
    int setControlThree(bool int1_enable, bool int1_boot, bool lh_active, bool pp_open,
                        bool data_readu_2, bool fifo_wter_2, bool fifo_over_2, bool fifo_empt_2);
    //int setControlFour();

private:
    /// L3GD20 REGISTERS
    int mWho;                       /// Device identification register

    int mControl_1;                 /// [ DR1 DR0 BW1 BW0 PD Zen Xen Yen ]
    int mControl_2;                 /// [ 0 0 HPM1 HPM0 HPC3 HPC2 HPC1 HPC0 ]
    int mControl_3;                 /// [ I1_INT1 I1_BOOT H_LACTIVE PP_OD I2_DRDY I2_WTM I2_ORUN I2_EMPTY ]
    int mControl_4;                 /// [ BDU BLE FS1 FS0 - 0 0 SIM ]
    int mControl_5;                 /// [ BOOT FIFO_EN - HPen INT1_SEL1 INT1_SEL0 OUT_SEL1 OUT_SEL0 ]

    int mReference;                 /// [ REF7 REF6 REF5 REF4 REF3 REF2 REF1 REF0 ]

    int mTemperatur;                /// [ TEMP7 TEMP6 TEMP5 TEMP4 TEMP3 TEMP2 TEMP1 TEMP0 ]

    int mStatus;                    /// [ ZYXOR ZOR YOR XOR ZYXDA ZDA YDA XDA ]

    int mOutXL;                     /// X axis angular rate data lsb: The value is expressed as two's complement
    int mOutXH;                     /// X axis angular rate data msb: The value is expressed as two's complement
    int mOutYL;                     /// Y axis angular rate data lsb: The value is expressed as two's complement
    int mOutYH;                     /// Y axis angular rate data msb: The value is expressed as two's complement
    int mOutZL;                     /// Z axis angular rate data lsb: The value is expressed as two's complement
    int mOutZH;                     /// Z axis angular rate data msb: The value is expressed as two's complement

    int mFifoControl;               /// [ FM2 FM1 FM0 WTM4 WTM3 WTM2 WTM1 WTM0 ]
    int mFifoControlSource;         /// [ WTM OVRN EMPTY FSS4 FSS3 FSS2 FSS1 FSS0 ]

    int mInteruptConfigure;         /// [ AND/OR LIR ZHIE ZLIE YHIE YLIE XHIE XLIE ]
    int mInteruptConfigureSource;   /// [ 0 IA ZH ZL YH YL XY XL ]

    int mInteruptXH;                /// [ - THSX14 THSX13 THSX12 THSX10 THSX9 THSX8 ]
    int mInteruptXL;                /// [ THSX7 THSX6 THSX5 THSX4 THSX3 THSX2 THSX1 THSX0 ]
    int mInteruptYH;                /// [ - THSY14 THSY13 THSY12 THSY10 THSY9 THSY8 ]
    int mInteruptYL;                /// [ THSY7 THSY6 THSY5 THSY4 THSY3 THSY2 THSY1 THSY0 ]
    int mInteruptzH;                /// [ - THSZ14 THSZ13 THSZ12 THSZ10 THSZ9 THSZ8 ]
    int mInteruptzl;                /// [ THSZ7 THSZ6 THSZ5 THSZ4 THSZ3 THSZ2 THSZ1 THSZ0 ]

    int mInteruptDuration;          /// [ WAIT D6 D5 D4 D3 D2 D1 D0 ]
};

#endif /// L3GD20_H
