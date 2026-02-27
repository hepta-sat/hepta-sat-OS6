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
    float bt_dec;
    char bt_hex[4];
    for(int i = 0; i < 10; i++) {
        eps.vol(&bt_dec);
        eps.vol_u16(bt_hex);
        pc_printf("Vol = %f\r\n",bt_dec);
        pc_printf("Vol_hex = %c%c%c%c\r\n",bt_hex[0],bt_hex[1],bt_hex[2],bt_hex[3]);
        ThisThread::sleep_for(1s);
    }
    while (1) {} // Keep the program running
}