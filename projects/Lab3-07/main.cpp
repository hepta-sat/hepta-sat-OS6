#include "mbed.h"
#include "HEPTA_CDH.h"
#include "HEPTA_EPS.h"
#include <cstdio>
#include <cstdarg>
HEPTA_CDH cdh(p5, p6, p7, p8, "sd");
HEPTA_EPS eps(p16,p26);
DigitalOut condition(LED1);
UnbufferedSerial gs(USBTX,USBRX,9600);
Timer sattime;
//command variable
volatile char rcmd = 0; // In Classic, int was used, but in OS6, char is sufficient
volatile int cmdflag = 0; // volatile is required because this variable can be modified by an interrupt

// printf-like wrapper for UnbufferedSerial
void gs_printf(const char* fmt, ...) {
    char buf[128];
    va_list args;
    va_start(args, fmt);
    int n = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    gs.write(buf, n);
}
//getting command and flag
void commandget()
{
    char temp;
    if (gs.read(&temp, 1) > 0) {
        rcmd = temp;   // Updated by interrupt handler
        cmdflag = 1;
    }
}
//interrupting 
void receive()
{
    gs.attach(&commandget, SerialBase::RxIrq);
}
//initialzing
void initialize()
{
    rcmd = 0;
    cmdflag = 0;
    condition = 0;
}

int main() 
{
    gs_printf("From Sat : Operation Start...\r\n");
    int flag = 0;      // condition
    float batvol,temp; //Voltage, Temerature
    sattime.start();
    receive(); //interupting by ground station command
    eps.turn_on_regulator();//turn on 3.3V conveter
    for(int i = 0; i < 100; i++) {
        //satellite condition led
        condition = !condition;
        
        //senssing HK data(dummy data)
        eps.vol(&batvol);
        temp   = 28.5;
        
        //Transmitting HK data to Ground Station(GS)
        float sattime_read = std::chrono::duration<float>(sattime.elapsed_time()).count();
        gs_printf("HEPTASAT::Condition = %d, Time = %f [s], batVol = %.2f [V],Temp = %.2f [C]\r\n",flag,sattime_read,batvol,temp);
        ThisThread::sleep_for(1000ms);
        
        //Power Saving Mode 
        if(batvol <= 3.5){
            eps.shut_down_regulator();
            gs_printf("Power saving mode ON\r\n"); 
            flag = 1;
        } else if((flag == 1) && (batvol > 3.7)) {
            eps.turn_on_regulator();
            gs_printf("Power saving mode OFF\r\n");
            flag = 0;
        }
        
        if (cmdflag == 1) {
            if (rcmd == 'a') {
               for(int j=0;j<5;j++){
                gs_printf("HEPTASAT::Hello World!\r\n");
                condition = 1;
                ThisThread::sleep_for(1000ms);
               }
            }else if(rcmd == 'b') {
                char str[100];
                if (!cdh.mount()) {
                    gs_printf("Failed to mount SD card.\r\n");
                    return 1;
                }
                mkdir("/sd/mydir", 0777);
                FILE *fp = fopen("/sd/mydir/test.txt","w");
                if(fp == NULL) {
                    error("Could not open file for write\r\n");
                }
                for(int i = 0; i < 10; i++) {
                    eps.vol(&batvol);
                    fprintf(fp,"%f\r\n",batvol);
                    ThisThread::sleep_for(1s);
                }
                fclose(fp);
                fp = fopen("/sd/mydir/test.txt","r");
                for(int j = 0; j < 10; j++) {
                    fgets(str,100,fp);
                    gs_printf("%s", str);
                }
                fclose(fp);
            }
            initialize();
        }
    }
    sattime.stop();
    gs_printf("From Sat : Operation Stop...\r\n");

    while (1) {} // Keep the program running
}