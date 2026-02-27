#include "mbed.h"

DigitalOut myleds[] = {LED1, LED2, LED3, LED4};

int main()
{
    while(true){
        for(int i = 0; i < 4; i++){
            myleds[i] = 1;
        }   
        ThisThread::sleep_for(200ms);
        for(int i = 0; i < 4; i++){
            myleds[i] = 0;
        }
        ThisThread::sleep_for(200ms);
    }
}