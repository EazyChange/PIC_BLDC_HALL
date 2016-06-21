//#include "DS18B20.h"
#include <pic18f4431.h>
#include "Delays.h"
#include "UART.h"


/*
#define NOP()       asm("nop")  
#define DQ 			RC0
#define DQ_IN() 	TRISC0=1//TRISA|=0x20 
#define DQ_OUT() 	TRISC0=0//TRISA&=~0x20

unsigned char TEMP[]={0x00,0x00};
//unsigned char temp_sign=0x00,
unsigned char temp_int=0x00,temp_df=0x00;


void WriteByte(unsigned char data)
{
	char i=0;
	for(i=8;i>0;i--)
	{
        DQ_OUT();
        DQ=0;
        //NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
        DQ=data & 0x01;
        delay(8);//8
        DQ=1;
        data>>=1;
        NOP();
    }
}
unsigned char ReadByte(void)
{
	unsigned char i,data=0x00;
    //DQ_OUT();DQ=1;NOP();
	for(i=8;i>0;i--)
	{
        DQ_OUT();
        DQ=0;
        data>>=1;
        DQ_IN();
        NOP();NOP();//NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
        if(DQ==1) 
        {
            data |= 0x80;
        }
        delay(7);//7
        NOP();
    }
    DQ_OUT();DQ=0;
    return data;
}
unsigned char INIT_DS18B20()
{
    unsigned char status;
    DQ_OUT();
    DQ=1;
    NOP();
    DQ=0;
    delay(122);
    DQ_IN();
    delay(9);///////////////////////
    status=DQ;
    delay(78);
    
    return status;
}

unsigned char GetTemp()
{
    if(INIT_DS18B20()!=0)
    {
        return 0;
    }
    else
    {
       // WriteByte(0xCC);		//因为只有一个DS18B20，所以跳过ROM
        //WriteByte(0x4E);		//温度转换
        
        WriteByte(0xCC);		//因为只有一个DS18B20，所以跳过ROM
        WriteByte(0x44);		//读暂存器
        delay(5);
        INIT_DS18B20();
        delay(5);
        WriteByte(0xCC);		//因为只有一个DS18B20，所以跳过ROM
        WriteByte(0xBE);
        delay(5);
      
        TEMP[0] = ReadByte();		//LSB在前
        TEMP[1] = ReadByte();	
     
        temp_int=((TEMP[1] << 4) & 0x70)+ ((TEMP[0] >> 4) & 0x0f); 
        temp_df=TEMP[0] & 0x07;
       
        return 1;
    }
}
*/