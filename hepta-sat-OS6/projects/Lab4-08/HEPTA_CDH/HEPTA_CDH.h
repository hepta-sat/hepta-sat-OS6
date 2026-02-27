#ifndef MBED_HEPTACDH_H
#define MBED_HEPTACDH_H
#include "mbed.h"
#include "FATFileSystem.h"
#include "SDBlockDevice.h"

class HEPTA_CDH {
public:
    HEPTA_CDH(PinName mosi, PinName miso, PinName sclk, PinName cs, const char* name);
    bool mount();   //Mount the SD card(true if success, false otherwise)
    void unmount(); //Unmount the SD card
    FATFileSystem* filesystem();

private:
    SDBlockDevice sd;
    FATFileSystem fs;
    bool _mounted = false;
};

#endif