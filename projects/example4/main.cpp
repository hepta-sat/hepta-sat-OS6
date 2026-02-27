#include "mbed.h"

DigitalOut myleds[] = {LED1, LED2, LED3, LED4};

int main()
{
    int i = 0;
    while(true){
        switch(i){
            case 1:
                myleds[0] = 1;
                break;
            case 2:
                myleds[1] = 1;
                break;
            default:
                myleds[2] = 1;
                break;    
        }
        i++;
        ThisThread::sleep_for(1s);
    }
}