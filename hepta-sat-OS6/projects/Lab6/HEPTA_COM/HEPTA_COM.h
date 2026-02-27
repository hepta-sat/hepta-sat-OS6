#ifndef MBED_HEPTAXBEE_H
#define MBED_HEPTAXBEE_H
#include "mbed.h"

class HEPTA_COM
{
public:
    UnbufferedSerial xbee;
    HEPTA_COM(PinName tx, PinName rx, int rate);
    volatile char rcmd; // In Classic, int was used, but in OS6, char is sufficient
    volatile int cmdflag; // volatile is required because this variable can be modified by an interrupt
//    void baud(int rate); 
    void xbee_receive();
    void initialize();
    void commandget();
    // void xbee_transmit(char dataA[],char dataB[],char dataC[],char dataD[],char dataE[],
    //                    char dataF[],char dataG[],char dataH[],char dataI[],char dataJ[],
    //                    char dataK[],char dataL[],char dataM[],char dataN[]);
    void puts( char *s );
    void printf(const char *format, ... );
    void putc(char data);
    char getc();

private:
    // UnbufferedSerial xbee;
};

#endif