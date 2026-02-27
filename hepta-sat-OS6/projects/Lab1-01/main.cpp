#include "mbed.h"
#include <cstdio>
#include <cstdarg>

UnbufferedSerial pc(USBTX, USBRX, 9600); 

// printf-like wrapper for UnbufferedSerial
void pc_printf(const char* fmt, ...) {
    char buf[128];            // Buffer for formatted string
    va_list args;
    va_start(args, fmt);
    int n = vsnprintf(buf, sizeof(buf), fmt, args); // Format the string
    va_end(args);
    pc.write(buf, n);         // Send the string as bytes via serial
}

int main(){
    int i1=7;
    float i2=7.5;
    char i3='H';
    char c;
    
    pc_printf("Hello World!\r\n");
    pc_printf("i1=%d\r\n", i1);
    pc_printf("i2=%f\r\n", i2);
    pc_printf("i3=%c\r\n", i3);
    pc.write(&i3, 1);
    pc_printf("\r\n");
    
    for(int j=0; j<10; j++){
        pc_printf("j=%d -- enter any key --> ", j);
        while (pc.read(&c, 1) == 0);
        pc.write(&c, 1);
        pc_printf("\r\n");
    }

    pc_printf("finish!!\r\n");

    while (1) {} // Keep the program running
}