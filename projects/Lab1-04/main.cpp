#include "mbed.h"
#include <cstdio>
#include <cstdarg>

UnbufferedSerial pc(USBTX,USBRX);
UnbufferedSerial sig(p13,p14);

int main() {
    char c;
    while(1) {
        while (pc.read(&c, 1) == 0);
        pc.write(&c, 1);
        sig.write(&c, 1);
    }
}