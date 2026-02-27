#include "mbed.h"

DigitalOut myleds[] = {LED1, LED2, LED3, LED4};

int main()
{
    while(true){
        myleds[0] = 1;
        ThisThread::sleep_for(200ms);
        myleds[0] = 0;
        ThisThread::sleep_for(200ms);
    }
}