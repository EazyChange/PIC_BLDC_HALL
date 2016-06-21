//-----------------------------------------------------------------
//  ����: DS1302 ʵʱʱ�ӳ���
//-----------------------------------------------------------------
#include <stdlib.h>
#include "DS1302.h"
#include <pic18f4431.h>

/*****************************************************************
��������delayms����
���ܣ���ʱ
��ڲ�����n
����ֵ����
******************************************************************/
/*
void delayms(int n)
{
  int i;
  for(i=n;i>0;i--);
}*/
/*****************************************************************
��������DS1302_INIT����
���ܣ�DS1302��ʼ��
��ڲ�������
����ֵ����
******************************************************************/
void DS1302_INIT(void)
{
    TRISD4=0;TRISE2=0;
    CLK = 0;                            //����ʱ���ź�
    RST = 0;                            //��λDS1302
    RST = 1;                            //ʹ��DS1302
    write_cmd(0x8e);                     //������д��������0x8e��ָ��д�����Ĵ���
    write_data(0x00);                    //��д�����Ĵ����������λΪ0������д��
    RST = 0;                            //��λDS1302  
}
/*****************************************************************
��������set_time����
���ܣ����õ�ǰʱ��
��ڲ�������
����ֵ����
******************************************************************/
void set_time(unsigned char *date)
{
    int i;
    RST=1;//ʹ��DS1302
    write_cmd(0xbe);//ʱ�Ӷ��ֽ�д����
    for(i=0;i<7;i++)
    {
     write_data(date[i]); //д��ʼʱ������
    }
    write_cmd(0x00);//����д������
    RST=0;//��λDS1302
}
/*****************************************************************
��������write_cmd����
���ܣ���DS1302д����
��ڲ�����cmd
����ֵ����
******************************************************************/
void write_cmd(unsigned char cmd)
{
    SET_IO_WR();
    int j;
    for(j=0;j<8;j++)
    {
        IO=0;//IO���0��DS1302д0
        CLK=0;//ʱ���ź�����
        if(cmd&0x01)
        {
            IO=1;
        }
        cmd=cmd>>1;
        CLK=1;//ʱ���ź����ߵ�ƽ    
    }
    CLK=0;;//ʱ���ź����͵�ƽ
}
/*****************************************************************
��������write_data����
���ܣ���DS1302д����
��ڲ�����data
����ֵ����
******************************************************************/
void write_data(unsigned char data)
{
    SET_IO_WR();
    int j;
    for(j=0;j<8;j++)
    {
        IO=0;//IO���0��DS1302д0
        CLK=0;//ʱ���ź�����
        if(data&0x01)
        {
            IO=1;
        }
        data=data>>1;
        CLK=1;//ʱ���ź����ߵ�ƽ    
    }
    CLK=0;//ʱ���ź����͵�ƽ
}
/*****************************************************************
��������read_time����
���ܣ���DS1302������
��ڲ�������
����ֵ��rec
******************************************************************/
unsigned char read_time(void)
{
    unsigned char rec;
    int j;                            //����ѭ������
    static bit temp;
    SET_IO_RD();                         //�������ݿڷ���Ϊ����
    for (j = 0;j < 8;j++)                  //������ȡ8bit
    {
        CLK = 0;                      //����ʱ���ź�
        temp=IO;
        rec = rec >> 1;          //���ռĴ�������1λ
        if(temp==1)
        {
         rec=rec|0x80;
        }
CLK = 1;                       //����ʱ���ź�
}
CLK = 0;                          //����ʱ���ź�
return (rec);                 //���ض�ȡ��������
}
/*****************************************************************
��������get_time����
���ܣ���ȡʵʱʱ��
��ڲ�������
����ֵ����
******************************************************************/
void get_time (void)
{ 
    int i;
    RST=1;//ʹ��DS1302
    write_cmd(0xbf);//ʱ�Ӷ��ֽڶ�����
    for(i=0;i<7;i++)
    {
        table[i]=read_time(); //��ȡʱ��
    }
    RST=0;
    //wyc201604026�����뵥λ���
    if(table[0]>=0x61)
    {
        RST=1;//ʹ��DS1302
        write_cmd(0xbe);//ʱ�Ӷ��ֽ�д����
        write_data(0x00);
        for(i=1;i<8;i++)
        {
            write_data(table[i]); //д��ʼʱ������
        }
        write_cmd(0x00);//����д������
        RST=0;//��λDS1302
    }
}
//weigh�������λ�������Ϊ4λ��ʱ��������Ϊ2λ������Ϊ1λ��
//asciiΪtable��ֵ��bufΪ�����ʵ�����ں�ʱ��
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
