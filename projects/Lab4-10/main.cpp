#include "mbed.h"
#include "HEPTA_EPS.h"
#include "HEPTA_CDH.h"
#include "HEPTA_SENSOR.h"
#include <cstdio>
#include <cstdarg>

UnbufferedSerial pc(USBTX,USBRX,9600);
HEPTA_EPS eps(p16,p26);
HEPTA_CDH cdh(p5,p6,p7,p8,"sd");
HEPTA_SENSOR sensor(p17,
                  p28,p27,0x19,0x69,0x13,
                  p13, p14,p25,p24);

// printf-like wrapper for UnbufferedSerial
void pc_printf(const char* fmt, ...) {
    char buf[128];
    va_list args;
    va_start(args, fmt);
    int n = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    pc.write(buf, n);
}

int main()
{
    if (!cdh.mount()) {
        pc_printf("Failed to mount SD card.\r\n");
        return 1;
    }
    FILE *dummy = fopen("/sd/dummy.txt","w");
    if(dummy == NULL) {
        error("Could not open file for write\r\n");
    }
    fclose(dummy);
    
    pc_printf("Camera Snapshot Mode\r\n");
    pc_printf("Hit Any Key To Take Picture\r\n");
    while(!pc.readable()) {
        ThisThread::sleep_for(10ms);
    }
    sensor.Sync();
    sensor.initialize(HeptaCamera_GPS::Baud115200, HeptaCamera_GPS::JpegResolution320x240);
    sensor.test_jpeg_snapshot_picture("/sd/test.jpg");
    while (1) {} // Keep the program running
}