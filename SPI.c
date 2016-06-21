#include <pic18f4431.h>
#include "SPI.h"

void INIT_SPI()
{
    TRISC4=1;TRISC5=0;TRISC7=0;
    SSPSTAT=0xc0;
    SSPCON=0x20;
    CKP=1;CKE=0;
    SMP=0;
}
void write_spi(unsigned char data)
{
    SSPBUF=data;
    while(!SSPIF);
    SSPIF=0;
}

unsigned char read_spi()
{
    unsigned char t;
    SSPBUF=0xff;
    while(!SSPIF);
    SSPIF=0;
    t=SSPBUF;
    return t;
}

