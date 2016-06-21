//#include <pic18f4431.h>
#include "UART.h"
#include "FRAM.h"
#include "motor.h"
#include <pic18f4431.h>
#include "SPI.h"

void Display_Menu()
{
    Send_Str("Welcome!\r\n");
    Send_Str("Please select:\r\n");
    Send_Str("R-- Read the Parameters.\r\n");
    Send_Str("O-- Read All Data.\r\n");
    Send_Str("S-- Set Default Parameters.\r\n");
    Send_Str("F-- Set ALL Data=0x00\r\n");
    Send_Str("*-- any other key to redispaly the menu.\r\n");
}
/*
unsigned char Read_Parameters()
{
    unsigned char res=Rev_Hex();
    switch (res)
    {
        case 'R':return 'R';break;
        case 'O':return 'O';break;
        case 'S':return 'S';break;
        case 'F':return 'F';break;
        default:Display_Menu();break;
    }
    return '*'; 
}*/
//设置默认参数不仅要给RAM发送数据还有给结构体发送，因为串口直接发送结构体数据，可以提高效率
void SetDefaultPara()
{
    write_F(0x1e,0x00);write_F(0x64,0x01);
    write_F(0x32,0x02);write_F(0x46,0x03);
    write_F(0x32,0x04);write_F(0x32,0x05);
    write_F(0x50,0x06);write_F(0x1e,0x07);
    write_F(0x64,0x08);write_F(0x0f,0x09);
    write_F(0x00,0x0a);write_F(0x1e,0x0b);
    Lev[0].duty=0x1e;Lev[0].hits=0x64;
	Lev[1].duty=0x32;Lev[1].hits=0x46;
    Lev[2].duty=0x32;Lev[2].hits=0x32;
    Lev[3].duty=0x50;Lev[3].hits=0x1e;
    Lev[4].duty=0x64;Lev[4].hits=0x0f;
}

void DRV_INIT()
{
    INIT_SPI();
    TRISE1=0;
    TRISA5=0;
    EN_GATE=1;
}

void delay(unsigned int t)
{
    while(t--);
}

void delay_n(unsigned char n)
{
    while(n--)
    {
        delay(65535);
    }
}
/*
void itoa_1(unsigned char buf[], unsigned char data, unsigned char i)
{
	if (data >= 0 && data < 10)
	{
		buf[0] = data + 0x30;
		buf[1] = '\0';
	}
	else if (data >= 10 && data < 100)
	{
		buf[0] = data / 10 + 0x30;
		buf[1] = data % 10 + 0x30;
		buf[2] = '\0';
	}
	else if (data >= 100 && data < 1000)
	{
		buf[0] = data / 100 + 0x30;
		buf[1] = data % 100 / 10 + 0x30;
		buf[2] = data % 10 + 0x30;
		buf[3] = '\0';
	}
	else
	{
		buf[0] = '\0';
	}
}
*/
