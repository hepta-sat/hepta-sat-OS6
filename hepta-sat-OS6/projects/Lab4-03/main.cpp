#include "mbed.h"
#include "HEPTA_EPS.h"
#include "HEPTA_SENSOR.h"
#include <cstdio>
#include <cstdarg>
 
UnbufferedSerial pc(USBTX,USBRX,9600);
HEPTA_EPS eps(p16,p26);
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
    float ax,ay,az;
    for(int i = 0; i<50; i++) {
        sensor.sen_acc(&ax,&ay,&az);
        pc_printf("acc : %f,%f,%f\r\n",ax,ay,az);
        ThisThread::sleep_for(1s);
    }
    while (1) {} // Keep the program running
}