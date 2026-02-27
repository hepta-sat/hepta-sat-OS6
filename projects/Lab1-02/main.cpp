#include "mbed.h"
#include <cstdio>
#include <cstdarg>
DigitalOut condition(LED1);
UnbufferedSerial gs(USBTX,USBRX,9600);
Timer sattime;

// printf-like wrapper for UnbufferedSerial
void gs_printf(const char* fmt, ...) {
    char buf[128];
    va_list args;
    va_start(args, fmt);
    int n = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    gs.write(buf, n);
}

int main()
{
    gs_printf("From Sat : Nominal Operation\r\n");
    int flag = 0; //condition flag
    float batvol, temp; //voltage, temperature 
    sattime.start();
    for(int i=0;i<50;i++){
        //satellite condition led
        condition = !condition;
        
        //senssing HK data(dummy data)
        batvol = 3.7;
        temp   = 28.5;
        
        //Transmitting HK data to Ground Station(GS)
        float sattime_read = std::chrono::duration<float>(sattime.elapsed_time()).count();
        gs_printf("HEPTASAT::Condition = %d, Time = %f [s], batvol = %2f [V], temp = %2f [deg C]\r\n",flag,sattime_read,batvol,temp);
        ThisThread::sleep_for(1000ms);
    }
    sattime.stop();
    gs_printf("From Sat : End of operation\r\n");

    while (1) {} // Keep the program running
}
