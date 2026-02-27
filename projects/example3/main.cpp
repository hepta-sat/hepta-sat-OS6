#include "mbed.h"

DigitalOut myleds[] = {LED1, LED2, LED3, LED4};

int main()
{
    int i = 0;
    while(true){
        if(i % 2 == 0){
            myleds[0] = 1;
            myleds[1] = 0;
        }else{
            myleds[0] = 0;
            myleds[1] = 1;
        }
        i++;
        ThisThread::sleep_for(1s);
    }
}
