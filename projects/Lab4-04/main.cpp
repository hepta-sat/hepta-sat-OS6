#include "mbed.h"
#include "HEPTA_EPS.h"
#include <cstdio>
#include <cstdarg>
#define GYRO 0x69<<1 //addr_gyro

UnbufferedSerial pc(USBTX, USBRX,9600);
HEPTA_EPS eps(p16,p26);
I2C i2c(p28,p27);

int16_t gyroscope[3];
float gx,gy,gz;
char cmd[2]={0};
uint8_t data[6]={0};
char send[1], rx[1];
char temp;

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
    pc_printf("gyroscope setting\r\n");
    cmd[0]=0x0F; 
    cmd[1]=0x04;
    i2c.write(GYRO,cmd,2); 
    cmd[0]=0x10; 
    cmd[1]=0x07;
    i2c.write(GYRO,cmd,2); 
    cmd[0]=0x11; 
    cmd[1]=0x00;
    i2c.write(GYRO,cmd,2); 
    while(1) {
        for(int i=0;i<6;i++){
            send[0]=(char)(2+i);
            i2c.write(GYRO,send,1);
            i2c.read(GYRO,rx,1);
            temp=rx[0];
            data[i]=temp;
        }
        for(int i=0;i<3;i++){
            gyroscope[i]=(int16_t)(((int16_t)data[i*2+1]<<8) | data[i*2]) >> 4;
            //if(gyroscope[i]>32767)gyroscope[i]-=65536; // This check is no longer needed.
        }
        gx = (float)gyroscope[0] * 125.0f / 2048.0f;
        gy = (float)gyroscope[1] * 125.0f / 2048.0f;
        gz = (float)gyroscope[2] * 125.0f / 2048.0f;
        pc_printf("gx = %2.4f, gy = %2.4f, gz = %2.4f\r\n",gx,gy,gz);
        ThisThread::sleep_for(1s);
    }
}