#include "mbed.h"
#include <cstdio>
#include <cstdarg>
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
//getting command and command flag
void commandget()
{
    char temp;
    if (gs.read(&temp, 1) > 0) {
        rcmd = temp;   // Updated by interrupt handler
        cmdflag = 1;
    }
}
//interrupting process by command receive
void receive()
{
    gs.attach(&commandget, SerialBase::RxIrq);
}
//initializing
void initialize()
{
    rcmd = 0;
    cmdflag = 0;
    condition = 0;
}

int main()
{
    gs_printf("From Sat : Nominal Operation\r\n");
    int flag = 0; //condition flag
    float batvol, temp; //voltage, temperature 
    sattime.start();
    receive(); //interrupting
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
        
        if(cmdflag == 1){
            if(rcmd == 'a'){
                //Please insert your answer



                
            }
            initialize(); //initializing
        }
    }
    sattime.stop();
    gs_printf("From Sat : End of operation\r\n");

    while (1) {} // Keep the program running
}