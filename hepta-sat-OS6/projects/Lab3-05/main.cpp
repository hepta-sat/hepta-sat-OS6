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

int main(){
    int flag = 0;
    float btvol;
    for(int i = 0; i < 100; i++) {
        //Get Battery Voltage
        eps.vol(&btvol);
        pc_printf("BatVol = %.2f [V]\r\n",btvol);
        //Power Saving Mode
        if(btvol <= 3.5){
            eps.shut_down_regulator();
            flag = 1;
        } else {
            eps.turn_on_regulator();
            flag = 0;
        }
        if(flag == 1){
            pc_printf("Power saving mode ON\r\n");
        }
        ThisThread::sleep_for(1s);
    }
    while (1) {} // Keep the program running
}
