#include <iostream>

#include <unistd.h>

/// ads1015 headers
#include <ads1015.h>


int setConfig(ADS1015 *ads1015)
{
    /// config msb : OS MUX PGA and MODE
    int status = ads1015->setOs(OSNoEffectOnWrite);
    if (status < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " setOs failed with error" << status << std::endl;
        return -1;
    }

    status = ads1015->setMux(MUXAIN0GND);
    if (status < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " setMux failed with error" << status << std::endl;
        return -2;
    }

    status = ads1015->setPga(PGA2048);
    if (status < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " setPga failed  with error " << status << std::endl;
        return -3;
    }

    status = ads1015->setMode(MODEContinous);
    if (status < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " setMode failed with error" << status << std::endl;
        return -4;
    }

    /// config lsb: DR, COMP_MODE, COMP_POL, COMP_LAT and COMP_QUE
    status = ads1015->setDr(DR128);
    if (status < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " setDr failed  with error " << status << std::endl;
        return -5;
    }

    status = ads1015->setCompMode(COMPMODEHysteresis);
    if (status < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " setCompMode failed  with error " << status << std::endl;
        return -6;
    }

    status = ads1015->setCompPol(COMPPOLActiveLow);
    if (status < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " setCompPol failed  with error " << status << std::endl;
        return -7;
    }

    status = ads1015->setCompLat(COMPLATNoneLatching);
    if (status < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " setCompLat failed  with error " << status << std::endl;
        return -8;
    }

    status = ads1015->setCompQue(COMPQUEDisable);
    if (status < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " setCompQue failed  with error " << status << std::endl;
        return -9;
    }

    status = ads1015->writeConfig();
    if (status < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " setConfig failed with error " << status << std::endl;
        return 10;
    }

    status = ads1015->initRead();
    if (status < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " initRead failed with error " << status << std::endl;
        return -11;
    }

    if ( 0 ) {
        std::cout << "os:        0x" << std::hex << ads1015->os()       << std::dec << std::endl;
        std::cout << "mux:       0x" << std::hex << ads1015->mux()      << std::dec << std::endl;
        std::cout << "pga:       0x" << std::hex << ads1015->pga()      << std::dec << std::endl;
        std::cout << "mode:      0x" << std::hex << ads1015->mode()     << std::dec << std::endl;
        std::cout << "dr:        0x" << std::hex << ads1015->dr()       << std::dec << std::endl;
        std::cout << "Comp mode: 0x" << std::hex << ads1015->compMode() << std::dec << std::endl;
        std::cout << "Comp pol:  0x" << std::hex << ads1015->compPol()  << std::dec << std::endl;
        std::cout << "Comp lat:  0x" << std::hex << ads1015->compLat()  << std::dec << std::endl;
        std::cout << "Comp que:  0x" << std::hex << ads1015->compQue()  << std::dec << std::endl;
        std::cout << std::dec << std::endl;
    }

    return 0;
}


int main(int argc, char **argv)
{
    float voltage = ConversionOutOfRange;

    ADS1015 ads1015((char*)FirmwareI2CDeviceses::i2c_1);

    int status = ads1015.openDevice();
    if (status < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " openDevice failed with error" << status << std::endl;
        return 0;
    }

    status = setConfig(&ads1015);
    if (status < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " setConfig failed with error " << status << std::endl;
        goto error;
    }

    usleep(20000);

    voltage = ads1015.readDevice();
    if (voltage == ConversionOutOfRange) {
        std::cerr << __func__ << ":" << __LINE__ << " readDevice failed with error " << status << std::endl;
        goto error;
    }

    std::cout << "voltage " << voltage << std::endl;

error:
    status = ads1015.closeDevice();
    if (status < 0)
        std::cerr << __func__ << ":" << __LINE__ << " closeDevice failed with error " << status << std::endl;

    /// return zero no matter
    return 0;
}
