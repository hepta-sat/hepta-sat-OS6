#include "mbed.h"
#include "HEPTA_COM.h"
#include <cstdio>
#include <cstdarg>

UnbufferedSerial pc(USBTX,USBRX,9600);
HEPTA_COM com(p9,p10,9600);

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
    pc_printf("Xbee Count Up Mode\r\n");
    for(int i = 0; i < 10; i++) {
        com.printf("num = %d\r\n",i);
        ThisThread::sleep_for(1s);
    }
    while (1) {} // Keep the program running
}
