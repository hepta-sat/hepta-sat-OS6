#include"Hepta9axis.h"
#include <cmath>   // for NAN
#define ACC 0x19<<1 //addr_accel
#define MAG 0x13<<1 //addr_compus
#define GYRO 0x69<<1 //addr_gyro

Hepta9axis::Hepta9axis(PinName sda, PinName scl, int aaddr, int agaddr, int amaddr) : n_axis(sda,scl),addr_accel(aaddr),addr_gyro(agaddr),addr_compus(amaddr)
{
    n_axis.frequency(100000);
    setup();
}

void Hepta9axis::setup()
{
    cmd[0]=0x14; //softreset
    cmd[1]=0xB6; //triggers a reset 
    n_axis.write(ACC,cmd,2); 
    cmd[0]=0x0F; //acceleration measurement range 
    cmd[1]=0x05; //+-4g
    n_axis.write(ACC,cmd,2);
    cmd[0]=0x11; //Selection of the main power modes and low power sleep period
    cmd[1]=0x00; //NORMAL mode, Sleep duration = 0.5ms
    n_axis.write(ACC,cmd,2);

    cmd[0]=0x0F; 
    cmd[1]=0x04;
    n_axis.write(GYRO,cmd,2);
    cmd[0]=0x10; 
    cmd[1]=0x07;
    n_axis.write(GYRO,cmd,2); 
    cmd[0]=0x11; 
    cmd[1]=0x00;
    n_axis.write(GYRO,cmd,2); 

    cmd[0]=0x4B; 
    cmd[1]=0x01;
    n_axis.write(MAG,cmd,2);
    cmd[0]=0x4C; 
    cmd[1]=0x00;
    n_axis.write(MAG,cmd,2);
    cmd[0]=0x4E; 
    cmd[1]=0x84;
    n_axis.write(MAG,cmd,2);
    cmd[0]=0x51; 
    cmd[1]=0x04;
    n_axis.write(MAG,cmd,2);
    cmd[0]=0x52; 
    cmd[1]=0x16;
    n_axis.write(MAG,cmd,2);
}

void Hepta9axis::sen_acc(float *ax,float *ay,float *az)
{
    int ret;
    //If sensor communication fails, data cannot be read.
    //This usually happens when the sensor power is OFF (power saving mode).
    send[0]=(char)(2); 
    ret = n_axis.write(ACC,send,1,true);
    if(ret != 0) goto nan_return;

    ret = n_axis.read(ACC,(char*)data,6);
    if(ret != 0) goto nan_return;

    for(int i=0;i<3;i++){
        accel[i]=(int16_t)(((int16_t)data[i*2+1]<<8) | data[i*2]) >> 4;
        if(accel[i]>2047)accel[i]-=4096;
    }
    *ax = (float)accel[0] / 512.0f * 9.8f;
    *ay = (float)accel[1] / 512.0f * 9.8f;
    *az = (float)accel[2] / 512.0f * 9.8f;
    return;

nan_return:
    // Sensor is OFF or communication failed
    *ax = *ay = *az = NAN;
}

void Hepta9axis::sen_gyro(float *gx,float *gy,float *gz)
{
    int ret;
    //If sensor communication fails, data cannot be read.
    //This usually happens when the sensor power is OFF (power saving mode).
    
    for(int i=0;i<6;i++){
        send[0]=(char)(2+i);
        ret=n_axis.write(GYRO,send,1);
        if(ret != 0) goto nan_return;
        ret=n_axis.read(GYRO,get,1);
        if(ret != 0) goto nan_return;
        temp=get[0];
        data[i]=temp;
    }
    for(int i=0;i<3;i++){
        gyroscope[i]=(int16_t)(((int16_t)data[i*2+1]<<8) | data[i*2]) >> 4;
        //if(gyroscope[i]>32767)gyroscope[i]-=65536;   //This check is unnecessary for int16_t values.
    }
    *gx = (float)gyroscope[0] * 125.0f / 2048.0f;
    *gy = (float)gyroscope[1] * 125.0f / 2048.0f;
    *gz = (float)gyroscope[2] * 125.0f / 2048.0f;
    return;

nan_return:
    // Sensor is OFF or communication failed
    *gx = *gy = *gz = NAN;
}

void Hepta9axis::sen_mag(float *mx,float *my,float *mz)
{
    cmd[0]=0x00;
    n_axis.write(MAG,cmd,1,1);
    n_axis.read(MAG,cmd,1);
    
    int ret;
    //If sensor communication fails, data cannot be read.
    //This usually happens when the sensor power is OFF (power saving mode).
    for(int i=0;i<8;i++){
        send[0]=(char)(0x42+i);
        ret=n_axis.write(MAG,send,1);
        if(ret != 0) goto nan_return;
        ret=n_axis.read(MAG,get,1);
        if(ret != 0) goto nan_return;
        temp=get[0];
        data[i]=temp;
    }
    for(int i=0;i<3;i++){
        if(i!=2)magnet[i]=(int16_t)(((int16_t)data[i*2+1]<<8) | data[i*2]) >> 3;
        else magnet[i]=(int16_t)(((int16_t)data[i*2+1]<<8) | data[i*2]) >> 1;
        if(i==2 && magnet[i]>16383)magnet[i]-=32768;
        else if(i!=2 && magnet[i]>4095)magnet[i]-=8092;
    }
    *mx = (float)magnet[0];
    *my = (float)magnet[1];
    *mz = (float)magnet[2];
    return;

nan_return:
    // Sensor is OFF or communication failed
    *mx = *my = *mz = NAN;
}