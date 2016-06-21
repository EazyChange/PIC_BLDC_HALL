#include "pic18f4431.h"
#include "SPI.h"
#include "Delays.h"
#include "FRAM.h"
#include "DS1302.h"
#include "motor.h"

/*HOLD and WP can be ignore*/
//#define DI_R  RC5
//#define DO_R  RC4
//#define CLK_R RC7


void FRAM_INIT()
{
    TRISD3=0;//CS
    INIT_SPI();
}
/*指定address地址，写入一个t*/
void write_F(unsigned char t,unsigned int address)
{
    unsigned char L=0x00;
    unsigned char H=0x00;
	H = (unsigned char)(address>> 8);
	L = (unsigned char)(address & 0x00ff);
    CS_R=0;
    write_spi(0x06);
    delay(100);
    CS_R=1;
    delay(100);
    CS_R=0;
    write_spi(0x02);
    write_spi(H);
    write_spi(L);
    write_spi(t);
    CS_R=1;
}
/*从指定地址都会一个值*/
unsigned char read_F(unsigned int address)
{
    unsigned char t;
    unsigned char L=0x00;
    unsigned char H=0x00;
	H = (unsigned char)(address>> 8);
	L = (unsigned char)(address & 0x00ff);
    CS_R=0;
    write_spi(0x03);
    write_spi(H);
    write_spi(L);
    t= read_spi();
    CS_R=1;
    return t;
}
//把起始地址到结束地址所有数据置为0x00
void DataClean(unsigned int b_add,unsigned int e_add)
{
    unsigned int i=0;
    for(i=b_add;i<e_add+1;i++)
    {
        write_F(0x00,i);
    }
}
extern unsigned char table[7];
extern unsigned char hits;
extern char OperatorID;
extern unsigned char UseNum;
extern struct MotorPara Lev[5];
void Fill_Data(unsigned char num)
{
    int address=16+(num-1)*8;
    write_F(OperatorID,address);write_F(Hex2Dex(table[6]),address+1);write_F(Hex2Dex(table[4]),address+2);write_F(Hex2Dex(table[3]),address+3);
    write_F(Hex2Dex(table[2]),address+4);write_F(Hex2Dex(table[1]),address+5);write_F(Hex2Dex(table[0]),address+6);write_F(Hex2Dex(UseNum),address+7);
}
/*
void write_reg(unsigned char t)
{
    write_spi(0x01);
    write_spi(t);
}
unsigned char read_reg()
{
    unsigned char t;
    write_spi(0x05);
    t=read_spi();
    return t;
}
*/