#include "mbed.h"
#include "HEPTA_EPS.h"
#include "HEPTA_SENSOR.h"
#include "HEPTA_COM.h"
#include <cstdio>
#include <cstdarg>

UnbufferedSerial pc(USBTX,USBRX,9600);
HEPTA_EPS eps(p16,p26);
HEPTA_SENSOR sensor(p17,
                  p28,p27,0x19,0x69,0x13,
                  p13, p14,p25,p24);
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
    int i = 0;
    float bt,ax,ay,az;
    com.printf("Count Up!\r\n");
    com.xbee_receive();
    while(1) {
        com.printf("num = %d\r\n",i);
        i++;
        ThisThread::sleep_for(1s);
        pc_printf("rcmd=%d, cmdflag=%d\r\n",com.rcmd, com.cmdflag);
        if (com.cmdflag == 1) {
            if (com.rcmd == 'a') {
                pc_printf("Command Get %d\r\n",com.rcmd);
                com.printf("HEPTA Uplink OK\r\n");
                pc_printf("===================\r\n");
                pc_printf("Accel sensing Mode\r\n");
                pc_printf("===================\r\n");
                for(int ii = 0; ii < 10; ii++) {
                    sensor.sen_acc(&ax,&ay,&az);
                    eps.vol(&bt);
                    com.printf("AX = %f\r\n",ax);
                    com.printf("AY = %f\r\n",ay);
                    com.printf("AZ = %f\r\n",az);
                    com.printf("V = %f\r\n",bt);
                    ThisThread::sleep_for(500ms);
                }
            }
            com.initialize();
        }
    }
}