#include "HEPTA_CDH.h"

HEPTA_CDH::HEPTA_CDH(PinName mosi, PinName miso, PinName sclk, PinName cs, const char* name)
    : sd(mosi, miso, sclk, cs), fs(name) {
}

bool HEPTA_CDH::mount() {
    if (_mounted) {
        return true;
    }
    int err = sd.init();
    if (err != 0) {
        return false;
    }
    err = fs.mount(&sd);
    if (err != 0 && err != -EBUSY) {
        sd.deinit();
        return false;
    }
    _mounted = true;
    return true;
}

void HEPTA_CDH::unmount() {
    fs.unmount();
    sd.deinit();
}

FATFileSystem* HEPTA_CDH::filesystem() {
    return &fs;
}
