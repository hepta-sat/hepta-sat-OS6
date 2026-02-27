#include "mbed.h"
#include "HEPTA_CDH.h"
#include "HEPTA_EPS.h"
#include <cstdio>
#include <cstdarg>
 
UnbufferedSerial pc(USBTX,USBRX,9600);



// printf-like wrapper for UnbufferedSerial
void pc_printf(const char* fmt, ...) {
    char buf[128];
    va_list args;
    va_start(args, fmt);
    int n = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    pc.write(buf, n);
} 
 
int main()
{

    float bt;
    char str[100];
    if (!cdh.mount()) {
        pc_printf("Failed to mount SD card.\r\n");
        return 1;
    }
    mkdir("/sd/mydir", 0777);
    FILE *fp = fopen("/sd/mydir/test.txt","w");
    if(fp == NULL) {
        error("Could not open file for write\r\n");
    }
    for(int i = 0; i < 10; i++) {
        

        
    }
    fclose(fp);
    fp = fopen("/sd/mydir/test.txt","r");
    for(int j = 0; j < 10; j++) {
        
        
    }
    fclose(fp);
    pc_printf("Goodbye!!\r\n");

    while (1) {} // Keep the program running
}
