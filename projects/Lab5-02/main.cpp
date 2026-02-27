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
    pc_printf("Xbee Uplink Ok Mode\r\n");
    com.xbee_receive();
    for(int i = 0; i < 10; i++) {
        com.printf("num = %d\r\n",i);
        pc_printf("rcmd=%d, cmdflag=%d\r\n",com.rcmd, com.cmdflag);
        if (com.cmdflag == 1) {
            if (com.rcmd == 'a') {
                pc_printf("Command Get %c\r\n", com.rcmd);
                com.printf("HEPTA Uplink OK\r\n");
            }
            com.initialize();
        }
        ThisThread::sleep_for(1s);
    }
    while (1) {} // Keep the program running
}