#include "mbed.h"
#include "HEPTA_EPS.h"
#include <cstdio>
#include <cstdarg>
#define ACC 0x19<<1 //addr_accel

UnbufferedSerial pc(USBTX,USBRX,9600);
HEPTA_EPS eps(p16,p26);
I2C i2c(p28,p27);

int16_t accel[3];
float ax,ay,az;
char cmd[2]={0};
uint8_t data[6]={0};
char send[1];

// printf-like wrapper for UnbufferedSerial
void pc_printf(const char* fmt, ...) {
    char buf[128];
    va_list args;
    va_start(args, fmt);
    int n = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    pc.write(buf, n);
}

int main() {
    i2c.frequency(100000);
    pc_printf("accel setting\r\n");
    cmd[0]=0x14; //softwareset 
    cmd[1]=0xB6; //triggers a reset
    i2c.write(ACC,cmd,2); 
    cmd[0]=0x0F; //acceleration measurement range
    cmd[1]=0x05; //+-4g
    i2c.write(ACC,cmd,2); 
    cmd[0]=0x11; //Selection of the main power modes and low power sleep period
    cmd[1]=0x00; //NORMAL mode, Sleep duration = 0.5ms
    i2c.write(ACC,cmd,2); 
    while(1) {
        send[0]=(char)(2);
        i2c.write(ACC,send,1,true);
        i2c.read(ACC,(char*)data,6);
        for(int i=0;i<3;i++){
            accel[i]=(int16_t)(((int16_t)data[i*2+1]<<8) | data[i*2]) >> 4;
            if(accel[i]>2047)accel[i]-=4096;
        }
        ax = (float)accel[0] / 512.0f * 9.8f;
        ay = (float)accel[1] / 512.0f * 9.8f;
        az = (float)accel[2] / 512.0f * 9.8f;
        pc_printf("ax = %2.4f, ay = %2.4f, az = %2.4f\r\n",ax,ay,az);
        ThisThread::sleep_for(1s);
    }
}