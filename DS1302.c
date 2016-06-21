//-----------------------------------------------------------------
//  名称: DS1302 实时时钟程序
//-----------------------------------------------------------------
#include <stdlib.h>
#include "DS1302.h"
#include <pic18f4431.h>

/*****************************************************************
函数名：delayms（）
功能：延时
入口参数：n
返回值：无
******************************************************************/
/*
void delayms(int n)
{
  int i;
  for(i=n;i>0;i--);
}*/
/*****************************************************************
函数名：DS1302_INIT（）
功能：DS1302初始化
入口参数：无
返回值：无
******************************************************************/
void DS1302_INIT(void)
{
    TRISD4=0;TRISE2=0;
    CLK = 0;                            //拉低时钟信号
    RST = 0;                            //复位DS1302
    RST = 1;                            //使能DS1302
    write_cmd(0x8e);                     //发控制写保护命令0x8e，指定写保护寄存器
    write_data(0x00);                    //当写保护寄存器数据最高位为0，允许写入
    RST = 0;                            //复位DS1302  
}
/*****************************************************************
函数名：set_time（）
功能：设置当前时间
入口参数：无
返回值：无
******************************************************************/
void set_time(unsigned char *date)
{
    int i;
    RST=1;//使能DS1302
    write_cmd(0xbe);//时钟多字节写命令
    for(i=0;i<7;i++)
    {
     write_data(date[i]); //写初始时间数据
    }
    write_cmd(0x00);//允许写入命令
    RST=0;//复位DS1302
}
/*****************************************************************
函数名：write_cmd（）
功能：向DS1302写命令
入口参数：cmd
返回值：无
******************************************************************/
void write_cmd(unsigned char cmd)
{
    SET_IO_WR();
    int j;
    for(j=0;j<8;j++)
    {
        IO=0;//IO输出0，DS1302写0
        CLK=0;//时钟信号拉低
        if(cmd&0x01)
        {
            IO=1;
        }
        cmd=cmd>>1;
        CLK=1;//时钟信号拉高电平    
    }
    CLK=0;;//时钟信号拉低电平
}
/*****************************************************************
函数名：write_data（）
功能：向DS1302写数据
入口参数：data
返回值：无
******************************************************************/
void write_data(unsigned char data)
{
    SET_IO_WR();
    int j;
    for(j=0;j<8;j++)
    {
        IO=0;//IO输出0，DS1302写0
        CLK=0;//时钟信号拉低
        if(data&0x01)
        {
            IO=1;
        }
        data=data>>1;
        CLK=1;//时钟信号拉高电平    
    }
    CLK=0;//时钟信号拉低电平
}
/*****************************************************************
函数名：read_time（）
功能：从DS1302读数据
入口参数：无
返回值：rec
******************************************************************/
unsigned char read_time(void)
{
    unsigned char rec;
    int j;                            //设置循环变量
    static bit temp;
    SET_IO_RD();                         //设置数据口方向为输入
    for (j = 0;j < 8;j++)                  //连续读取8bit
    {
        CLK = 0;                      //拉低时钟信号
        temp=IO;
        rec = rec >> 1;          //接收寄存器右移1位
        if(temp==1)
        {
         rec=rec|0x80;
        }
CLK = 1;                       //拉高时钟信号
}
CLK = 0;                          //拉低时钟信号
return (rec);                 //返回读取到的数据
}
/*****************************************************************
函数名：get_time（）
功能：获取实时时间
入口参数：无
返回值：无
******************************************************************/
void get_time (void)
{ 
    int i;
    RST=1;//使能DS1302
    write_cmd(0xbf);//时钟多字节读命令
    for(i=0;i<7;i++)
    {
        table[i]=read_time(); //获取时间
    }
    RST=0;
    //wyc201604026放置秒单位溢出
    if(table[0]>=0x61)
    {
        RST=1;//使能DS1302
        write_cmd(0xbe);//时钟多字节写命令
        write_data(0x00);
        for(i=1;i<8;i++)
        {
            write_data(table[i]); //写初始时间数据
        }
        write_cmd(0x00);//允许写入命令
        RST=0;//复位DS1302
    }
}
//weigh是输出的位数，年份为4位，时分秒月日为2位，星期为1位。
//ascii为table的值，buf为输出的实际日期和时间
void Ascii2Str(unsigned char ascii,unsigned char buf[])//,int weigh)
{
    //if(weigh==2)
    {
        if (ascii <= 0x09 && ascii >= 0x00)
        {
            buf[0] = '0';
            buf[1] = ascii + 0x30;
            buf[2]='\0';
        }
        else
        {
            buf[0] = ((ascii & 0xf0)>>4) + 0x30;
            buf[1] = (ascii & 0x0f) + 0x30;
            buf[2]='\0';
        }
    }/*
    if(weigh==4)
    {
        if (ascii <= 0x09 && ascii >= 0x00)
        {
            buf[0] = '2';
            buf[1] = '0';
            buf[2] = '0';
            buf[3] = ascii + 0x30;
            buf[4]='\0';
        }
        else
        {
            buf[0] = '2';
            buf[1] = '0';
            buf[2] = ((ascii & 0xf0)>>4) + 0x30;
            buf[3] = (ascii & 0x0f) + 0x30;
            buf[4]='\0';
        }
    }
	if(weigh==1)
    {
        if (ascii <= 0x09 && ascii >= 0x00)
        {
            buf[0] = ascii + 0x30;
            buf[1]='\0';
        }
    }*/
}

unsigned char Hex2Dex(unsigned char Hex)
{
    unsigned char res=0x00,tmp=0x00;
    if(Hex<=0x60)
    {
        res=Hex/16;
        tmp=Hex%16;
    }
    return res*10+tmp;
}

void TimeFormat( char *ascii,  char min,  char max)//,int mode)
{
	if (*ascii< min || *ascii==0xff)
	{
		*ascii = max;
		return;
	}
    if (*ascii > max)
	{
		*ascii = min;
		return;
	}
    /*
	if (mode == 0)
	{
		if ((*ascii & 0x0f) > 0x09)
		{
			*ascii = (*ascii & 0xf0) + 0x10;
		}
	}
	else
	{
		if ((*ascii & 0x0f) > 0x09)
		{
			*ascii = 0x09 + (*ascii & 0xf0);
		}
	}*/
    char buf=*ascii & 0x0f;
    if (buf == 0x0a)
	{
			*ascii = (*ascii & 0xf0) + 0x10;
	}
    if(buf == 0x0f)
    {
        *ascii = 0x09 + (*ascii & 0xf0);
    }
}
