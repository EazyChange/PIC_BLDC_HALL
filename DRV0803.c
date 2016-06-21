#include <pic18f4431.h>
#include "DRV0803.h"
#include "SPI.h"

void DRV_INIT()
{
    INIT_SPI();
    TRISE1=0;
    TRISA5=0;
    EN_GATE=1;
}