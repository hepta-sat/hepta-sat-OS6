#include "mbed.h"
#include "HEPTA_EPS.h"
#include <cstdio>
#include <cstdarg>
 
UnbufferedSerial pc(USBTX,USBRX,9600);
HEPTA_EPS eps(p16,p26);

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
    float bt;
    for(int i = 0; i < 10; i++) {
        eps.vol(&bt);
        pc_printf("Vol = %f\r\n",bt);
        ThisThread::sleep_for(1s);
    }
    while (1) {} // Keep the program running
}