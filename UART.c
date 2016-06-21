#include <pic18f4431.h>
//#include <stdio.h>
#include <string.h>

void UART_INIT()
{
    TRISC7=1;
    TRISC6=1;
    //TRISC1=1;
    SPBRG=50;
    SYNC=0;BRG16=0;BRGH=1;
    
    TXSTA=0x24;//0x24  
    RCSTA=0x90;//1001
    //WUE=1;
    RCIF=0;
    //RCIE=0;
    //TXIE=0;
    //PEIE=1;
    //GIE=1;
}

void Send_Hex(unsigned char data)
{
    TXREG=data;
    while(!TRMT);
    RCIF=0;
}

void Send_Str(unsigned char *data)
{
    int i=0;
    for(i=0;i<strlen(data);i++)
    {
      Send_Hex(data[i]); 
    }  
}
unsigned char Rev_Hex()
{
    unsigned char temp;
    if(RCIF==1)
    {
        OERR=0;
        CREN=1;
        temp=RCREG;
    }
    return temp;
}
