#include "mbed.h"
#include "HEPTA_CDH.h"
#include "HEPTA_EPS.h"
#include "HEPTA_SENSOR.h"
#include "HEPTA_COM.h"
#include <cstdio>
#include <cstdarg>
HEPTA_CDH cdh(p5, p6, p7, p8, "sd");
HEPTA_EPS eps(p16,p26);
HEPTA_SENSOR sensor(p17,
                  p28,p27,0x19,0x69,0x13,
                  p13, p14,p25,p24);
HEPTA_COM com(p9,p10,9600);
DigitalOut condition(LED1);
UnbufferedSerial sat(USBTX,USBRX,9600);
Timer sattime;

// printf-like wrapper for UnbufferedSerial
void sat_printf(const char* fmt, ...) {
    char buf[128];
    va_list args;
    va_start(args, fmt);
    int n = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    sat.write(buf, n);
}

int main() {
    sat_printf("From Sat : Nominal Operation\r\n");
    com.printf("From Sat : Nominal Operation\r\n");
    int flag = 0; //condition flag
    float batvol, temp; //voltage, temperature
    sattime.start();
    com.xbee_receive();//interupting by ground station command
    eps.turn_on_regulator();//turn on 3.3V conveter
    ThisThread::sleep_for(50ms);
    sensor.setup();
    for(int i = 0; i < 100; i++) {       
        //satellite condition led
        condition = !condition;
        
        //senssing HK data(dummy data)
        eps.vol(&batvol);
        sensor.temp_sense(&temp);
        
        //Transmitting HK data to Ground Station(GS)
        float sattime_read = std::chrono::duration<float>(sattime.elapsed_time()).count();
        com.printf("HEPTASAT::Condition = %d, Time = %f [s], batVol = %.2f [V],Temp = %.2f [C]\r\n",flag,sattime_read,batvol,temp);
        ThisThread::sleep_for(1000ms);
                
        //Power Saving Mode 
        if((batvol <= 3.5)  || (temp > 35.0)){
            //In this mode, the sensors are powered OFF.
            //Sensor data cannot be read.
            eps.shut_down_regulator();
            com.printf("Power saving mode ON (Sensors are OFF)\r\n"); 
            flag = 1;
        } else if((flag == 1) && (batvol > 3.7) && (temp <= 25.0)) {
            eps.turn_on_regulator();
            com.printf("Power saving mode OFF\r\n");
            ThisThread::sleep_for(50ms);
            sensor.setup();
            flag = 0;
        }
        //Contents of command
        if (com.cmdflag == 1) {
            if (com.rcmd == 'a') {
                sat_printf("rcmd=%c,cmdflag=%d\r\n",com.rcmd,com.cmdflag);
                com.printf("Hepta-Sat Lite Uplink Ok\r\n");
                for(int j=0;j<5;j++){
                    com.printf("Hello World!\r\n");
                    condition = 1;
                    ThisThread::sleep_for(1000ms);
                }
            }else if (com.rcmd == 'b') {
                sat_printf("rcmd=%c,cmdflag=%d\r\n",com.rcmd,com.cmdflag);
                com.printf("Hepta-Sat Lite Uplink Ok\r\n");
                char str[100];
                if (!cdh.mount()) {
                    sat_printf("Failed to mount SD card.\r\n");
                    return 1;
                }
                mkdir("/sd/mydir", 0777);
                FILE *fp = fopen("/sd/mydir/satdata.txt","w");
                if(fp == NULL) {
                    error("Could not open file for write\r\n");
                }
                for(int i = 0; i < 10; i++) {
                    eps.vol(&batvol);
                    fprintf(fp,"%f\r\n",batvol);
                    condition = 1;
                    ThisThread::sleep_for(1000ms);
                }
                fclose(fp);
                fp = fopen("/sd/mydir/satdata.txt","r");
                for(int i = 0; i < 10; i++) {
                    fgets(str,100,fp);
                    com.puts(str);
                }
                fclose(fp);                
            }else if (com.rcmd == 'c') {        
                sat_printf("rcmd=%c,cmdflag=%d\r\n",com.rcmd,com.cmdflag);
                com.printf("Hepta-Sat Lite Uplink Ok\r\n");
                float ax,ay,az;
                for(int i = 0; i<10; i++) {
                    sensor.sen_acc(&ax,&ay,&az);
                    com.printf("acc : %f,%f,%f\r\n",ax,ay,az);
                    ThisThread::sleep_for(1000ms);
                }
            }else if (com.rcmd == 'd') {
                sat_printf("rcmd=%c,cmdflag=%d\r\n",com.rcmd,com.cmdflag);
                com.printf("Hepta-Sat Lite Uplink Ok\r\n");
                float gx,gy,gz;
                for(int i = 0; i<10; i++) {
                    sensor.sen_gyro(&gx,&gy,&gz);
                    com.printf("gyro: %f,%f,%f\r\n",gx,gy,gz);
                    ThisThread::sleep_for(1000ms);
                }
            }else if (com.rcmd == 'e') {
                //Please insert your answer

            }
            com.initialize();
        }
    }
    sattime.stop();
    sat_printf("From Sat : End of operation\r\n");
    com.printf("From Sat : End of operation\r\n");

    while (1) {} // Keep the program running
}
