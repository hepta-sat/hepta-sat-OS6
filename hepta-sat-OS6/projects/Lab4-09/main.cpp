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
    sensor.gps_setting();
    ThisThread::sleep_for(500ms);
    int quality=0,stnum=0,gps_check=0;
    char ns='A',ew='B',aunit='m';
    float time=0.0,latitude=0.0,longitude=0.0,hacu=0.0,altitude=0.0;
    for(int i=1; i<1000; i++) {
        sensor.gga_sensing(&time, &latitude, &ns, &longitude, &ew, &quality, &stnum, &hacu, &altitude, &aunit, &gps_check);
        if((gps_check==0)||(gps_check==1)) {
            pc_printf("GPGGA,%f,%f,%c,%f,%c,%d,%d,%f,%f,%c\r\n",time,latitude,ns,longitude,ew,quality,stnum,hacu,altitude,aunit);
        }
        ThisThread::sleep_for(200ms);
    }
    while (1) {} // Keep the program running
}