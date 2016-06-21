/*
 * File:   main.c
 * Author: Administrator
 *
 * Created on 2015年10月29日, 下午4:09
 */
/////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <pic18f4431.h>
#include "PWM.h"
#include "DS18B20.h"
#include "UART.h"
#include "SPI.h"
#include "LCD.h"
#include "FRAM.h"
#include "AD.h"
#include "motor.h"
#include "DS1302.h"
#include "key.h"

#include <xc.h>
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
// CONFIG1H
#pragma config OSC = HS //IRCIO      // Oscillator Selection bits (XT oscillator)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)
#pragma config IESO = ON        // Internal External Oscillator Switchover bit (Internal External Switchover mode enabled)

// CONFIG2L
#pragma config PWRTEN = OFF     // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bits (Brown-out Reset enabled)
// BORV = No Setting

// CONFIG2H
#pragma config WDTEN = OFF       // Watchdog Timer Enable bit (WDT enabled)
#pragma config WDPS = 32768     // Watchdog Timer Postscale Select bits (1:32768)
#pragma config WINEN = OFF      // Watchdog Timer Window Enable bit (WDT window disabled)

// CONFIG3L
#pragma config PWMPIN = ON     // PWM output pins Reset state control (PWM outputs disabled upon Reset (default))
#pragma config LPOL = HIGH      // Low-Side Transistors Polarity (PWM0, 2, 4 and 6 are active-high)
#pragma config HPOL = HIGH      // High-Side Transistors Polarity (PWM1, 3, 5 and 7 are active-high)
#pragma config T1OSCMX = OFF     // Timer1 Oscillator MUX (Low-power Timer1 operation when microcontroller is in Sleep mode)

// CONFIG3H
#pragma config FLTAMX = RC1     // FLTA MUX bit (FLTA input is multiplexed with RC1)
#pragma config SSPMX = RC7      // SSP I/O MUX bit (SCK/SCL clocks and SDA/SDI data are multiplexed with RC5 and RC4, respectively. SDO output is multiplexed with RC7.)
#pragma config PWM4MX = RD5     // PWM4 MUX bit (PWM4 output is multiplexed with RB5)
#pragma config EXCLKMX = RC3    // TMR0/T5CKI External clock MUX bit (TMR0/T5CKI external clock input is multiplexed with RC3)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (Enabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Low-Voltage ICSP Enable bit (Low-voltage ICSP enabled)

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000200-000FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (001000-001FFF) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (002000-002FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (003000-003FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot Block (000000-0001FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000200-000FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (001000-001FFF) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (002000-002FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (003000-003FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0001FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000200-000FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (001000-001FFF) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (002000-002FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (003000-003FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0001FFh) not protected from table reads executed in other blocks)
/***************************************************************************************************************************************************************/
/***************************************************************************************************************************************************************/
/****************************以上为控制位和头文件设置*************************************************************************************************************/

extern unsigned char table[7];//DS1302的输出时间数组，分别为 秒  分  时  日  月  星期  年
extern short Speed;//电机转速
short LastSpeed=0;
unsigned char SL,SH;
extern bit MOTOR_BUTTON;//电机开关
extern bit MOTOR_DIR;//电机方向
extern unsigned char STATE_HALL[6];//霍尔正转状态
extern unsigned char STATE_HALL_BACK[6];//霍尔反转状态
extern unsigned char hits;//实际打击次数
extern bit OverTEMP;//过温
extern unsigned char temp_int,temp_df;//DS18B20温度参数
extern unsigned char UseNum;

/*默认参数*/
//bit SerialOpen=0;//0是关闭 1是开
char OperatorID=1;//操作人员编号
unsigned char OpenTimes=0;//开机次数
unsigned char OpenTimes_tmp=0;//开机次数
//extern unsigned char RAM_Data[16];//RAM中前16位数据
int address=0;//使用在RAM中的临时地址变量
char Page=1;//初始化指向页面
bit Uart_Flag=0;

int main(int argc, char** argv)
{
	/*内部时钟8M使能*/
	//OSCTUNE=0x3f;
	//OSCCON=0xff;

	ANSEL0=0x03;//温度传感器和电源电压采集
	ANSEL1=0x00;//其他AD口全部设为普通IO
	//打开系统中断
	GIE=1;
	PEIE=1;
    //临时变量
	char Arrow_Ptr=0;//初始化箭头指向
	//读取RAM数据
    /*RAM前16个单元内存分配
    0   1   2   3   4   5   6   7   8   9       10      11  12  13  14  15
    无                                        开机次数  1档占空比
     */
	FRAM_INIT();
	for(address=0; address<5; address++)
	{
        Lev[address].duty=read_F(address*2);
        Lev[address].hits=read_F(address*2+1);
	}//读前10个数据，放进5个结构体里 
    Lev[0].duty=read_F(11);//由于RAM第1个单元有问题，所以放在11存储单元
    OpenTimes_tmp=OpenTimes=read_F(10);//读开机次数
    //初始化各种功能寄存器
    TRISE1=0;EN_GATE=1;//驱动板使能位
	PWM_INIT();
	LCD_INIT();
	DispLogo();
	DS1302_INIT();
	CAP_INIT();
	AD_INIT();
    //T0CON=0x84;TMR0IE=1;TMR0IF=0;TMR0IP=0;//定时器初始化
    delay_n(12);//界面延迟
	LCD_CLS();
	//电机参数设置
	MOTOR_BUTTON = 0;//默认电机关
    MOTOR_DIR=0;//默认正转
	extern struct MotorPara SetPara[5];
    SetPara[0]=Lev[0];SetPara[1]=Lev[1];SetPara[2]=Lev[2];SetPara[3]=Lev[3];SetPara[4]=Lev[4];//定义在界面中显示和更改的结构体，储存和修改占空比和打击次数
	char SetLev=1;//默认为1档
	//外触发INT0中断设置
    //TMR0ON=0;
    //T0CS=0;
	TRISC3=1;
	//INTEDG0=1;//下降沿触发
    //INT0IF=0;
	//INT0IE=1;//开串口中断
    //按键初始化
	KEY_INIT();
	unsigned char Key=0; 
	while(1)
	{
			Key=KeyScan();//按键扫描
			if(Page==1)//操作编号选择
			{
                MOTOR_BUTTON=0;
				get_time();//table里面已经包括了时间信息
				IDPage();
				if(Key==1)//增加人员编号
				{
					Key=0;
					if(++OperatorID>10)
                    {
                        OperatorID=1;
					}
				}
				if(Key==2)//减少人员编号
				{
					Key=0;
					if(--OperatorID<1)
                    {
                        OperatorID=10;
					}
				}
				if(Key==3)//离开此界面
				{
					Key=0;
                    //FRAM_INIT();
                    OpenTimes=OpenTimes_tmp;//可以保证，每次开机无论经过多少次都记录一个位置
                    if(OpenTimes>0xFD)//如果开机次数大于253，则开机次数复位为0
                    {
                        write_F(0,0x0a);
                    }
                    else//否则开机次数增加
                    {
                        OpenTimes++;
                        write_F(OpenTimes,0x0a);
                    }
                    Fill_Data(OpenTimes==30?254:OpenTimes);//把本次开机的信息写入RAM
                    LCD_CLS();
					Page=2;
				}
			}
			if(Page==2)//操作界面
			{
				DispPage1(SetPara[SetLev-1],SetLev);
                SET_DUTY(SetPara[SetLev-1].duty);//设置占空比
                Speed=(625000.0/(SL+SH*256.0));//计算转速，此公式为2对磁极的无刷电机。即转一圈有4个6步换向周期
                readAD();//AD采集，包括电源电压和温度传感器
                if(Speed-LastSpeed<=-300)
                {
                    hits++;
                    if(hits>=100)
                    {
                        hits=100;
                    }
                }
                LastSpeed=Speed;
                if(Key==1)//加档
				{
					Key=0;
					if(++SetLev>5)
                    {
                        SetLev=5;//如果增大到5档，就不能回到1档，否则对电机不好
                    }
                    hits=0;
				}
				if(Key==2)//减档
				{
					Key=0;
					if(--SetLev<1)
                    {
                        SetLev=1;
                    }
                    hits=0;
				}
                if(Key==3)//开电机
                {
                    Key=0;
                    UseNum++;
                    write_F(UseNum,16+(OpenTimes==30?254:OpenTimes-1)*8+7);
                    Speed=0;
                    //hits++;
                    MOTOR_BUTTON=1;
                }
                if(Key==4)//关电机
                {
                    Key=0; 
                    Speed=0;
                    MOTOR_BUTTON=0;
                }
                if(MOTOR_BUTTON==0)//电机关闭后才允许正反转改变
                {
                    if(RC2==0)
                    {
                        MOTOR_DIR=0;
                    }
                    if(RC1==0)
                    {
                        MOTOR_DIR=1;
                    }
                }
				if(Key==7)//退出界面
				{
					Key=0;
					Page=3;
					LCD_CLS();
				}               
			}
			if(Page==3)//运行参数设置界面
			{
                hits=0;
                MOTOR_BUTTON=0;
				SetPage(SetPara[SetLev-1],SetLev,Arrow_Ptr);
				if(Key==3)
				{
					Key=0;
					Arrow_Ptr=++Arrow_Ptr % 3;
				}
				if(Arrow_Ptr==0)//档位
				{
					if(Key==1)
					{
						Key=0;
						if(++SetLev>5)
                        {
                            SetLev=1;
                        }
					}
					if(Key==2)
					{
						Key=0;
                        --SetLev;
						if(SetLev<1  || SetLev==0xff)
                        {
                            SetLev=5;
                        }
					}
				}
				if(Arrow_Ptr==1)//占空比
				{
					if(Key==1)
					{
						Key=0;
						if(++SetPara[SetLev-1].duty>100)
                        { 
                            SetPara[SetLev-1].duty=0;
                        }
					}
					if(Key==2)
					{
						Key=0;
                        --SetPara[SetLev-1].duty;
						if(SetPara[SetLev-1].duty<1 || SetPara[SetLev-1].duty==0xff)
                        {
                            SetPara[SetLev-1].duty=100;
                        }	
					}
				}
				if(Arrow_Ptr==2)//打击次数
				{
					if(Key==1)
					{
						Key=0;
                        if(++SetPara[SetLev-1].hits>100)
                        {
                            SetPara[SetLev-1].hits=0;
                        }
					}
					if(Key==2)
					{
						Key=0;
                        --SetPara[SetLev-1].hits;
						if(SetPara[SetLev-1].hits<1 || SetPara[SetLev-1].hits==0xff)
                        {
                            SetPara[SetLev-1].hits=100;
                        }
					}
				}
				if(Key==4)//设置成功
				{
					//FRAM_INIT();
                    for(address=0; address<5; address++)
                    {
                        write_F(SetPara[address].duty, 2*address==0x00?0x0b:(2*address));
                        write_F(SetPara[address].hits, 2*address+1);
                        Lev[address]=SetPara[address];
                        //以上所有设置都赋给RAM和结构体
                    }
					//LCD_INIT();
                    LCD_CLS();
					SetOKPage();
					delay_n(6);
					LCD_CLS();
				}
				if(Key==7)//离开此界面
				{
					Key=0;
					Arrow_Ptr=0;
					Page=4;
					LCD_CLS();
				}
			}
			if(Page==4)//时间日期设置界面
			{
                MOTOR_BUTTON=0;
                TimePage(table[6],table[4],table[3],table[2],table[1],table[0],Arrow_Ptr);
				if(Key==3)
				{
					Arrow_Ptr=++Arrow_Ptr % 6;
				}
                //20160414 wyc
                int index=Arrow_Ptr==0 ? 6 :(5-Arrow_Ptr);
                if(Key==1)
                {
                    ++table[index];
                }
                if(Key==2)
                {
                    --table[index];
                }
                switch (Arrow_Ptr)
                {
                    case 0:TimeFormat(&table[index],0x00,0x99);break;
                    case 1:TimeFormat(&table[index],0x01,0x12);break;
                    case 2:TimeFormat(&table[index],0x01,0x31);break;
                    case 3:TimeFormat(&table[index],0x00,0x24);break;
                    default:TimeFormat(&table[index],0x00,0x60);break;
                }
                ////////////////////////////////////////////////////////////////
				if(Key==4)//设置成功
				{
					set_time(table);
					LCD_CLS();
					SetOKPage();
					delay_n(6);
					LCD_CLS();
				}
				if(Key==7)//离开此界面
				{
					Key=0;
					Arrow_Ptr=0;
					Page=5;
					LCD_CLS();
				}
			}
			if(Page==5)//出厂设置界面
			{
                MOTOR_BUTTON=0;
				ResetPage(Arrow_Ptr);
				if(Key==3)
				{
					Key=0;
					Arrow_Ptr=++Arrow_Ptr % 3;
				}
				if(Key==4)
				{
					if(Arrow_Ptr==0)//默认参数设置
					{
                        //FRAM_INIT();
						SetDefaultPara();
						//Lev[0].duty=0x1e;Lev[0].hits=0x64;
                        SetPara[0].duty=0x1e;SetPara[0].hits=0x64;
						//Lev[1].duty=0x32;Lev[1].hits=0x46;
                        SetPara[1].duty=0x32;SetPara[1].hits=0x46;
						//Lev[2].duty=0x32;Lev[2].hits=0x32;
                        SetPara[2].duty=0x32;SetPara[2].hits=0x32;
						//Lev[3].duty=0x50;Lev[3].hits=0x1e;
                        SetPara[3].duty=0x50;SetPara[3].hits=0x1e;
						//Lev[4].duty=0x64;Lev[4].hits=0x0f;
                        SetPara[4].duty=0x64;SetPara[4].hits=0x0f;
						OpenTimes=OpenTimes_tmp=0;

						//LCD_INIT();
                        LCD_CLS();
						SetOKPage();
						delay_n(6);
						LCD_CLS();
					}
					if(Arrow_Ptr==1)//默认时间设置
					{
						extern unsigned char start[7];
						set_time(start);
						//LCD_INIT();
                        LCD_CLS();
						SetOKPage();
						delay_n(6);
						LCD_CLS();
					}
					if(Arrow_Ptr==2)//清空所有数据
					{
                        //FRAM_INIT();
						DataClean(0x10,0x8ff);
						//LCD_INIT();
                        LCD_CLS();
						SetOKPage();
						delay_n(6);
						LCD_CLS();
					}
				}
				if(Key==7)//离开此界面
				{
					Key=0;
					Arrow_Ptr=0;
					Page=1;
					LCD_CLS();
				}
			}  
		if(MOTOR_BUTTON==1 && OverTEMP==0)
		{
            unsigned char change=(PORTA >>2) & 0x07;
            if(MOTOR_DIR==0)
            {
                OVDCOND=STATE_HALL[change-0x01];
            }
            else
            {
                OVDCOND=STATE_HALL_BACK[change-0x01];
            }
		}
		else
		{
            Speed=0;
			OVDCOND=0x00;
        }
        if(RC3==0)
        {
            UART_INIT();
            SSPEN=0;//禁止spi
            Display_Menu();
            RCIE=1;RCIF=0;
        }
	}
	return 0;
}

extern unsigned char AllData[8];
void interrupt ISR(void)
{/*
	if(INT0IF==1)//使能串口，禁止spi
	{
            INT0IF=0;
            UART_INIT();
            SSPEN=0;//禁止spi
            Display_Menu();
            RCIE=1;RCIF=0;
            INT0IE=0;
            Uart_Flag=1;
	}*/
	if(RCIF==1)//串口接收中断
	{
        RCIF=0;
		switch (RCREG)
		{//Read the Parameters
		case 'R'://不用访问RAM，直接访问当前RAM_Data数组
		{
            RCIE=0;
			for(address=0; address<5; address++)
			{
                Send_Hex(Lev[address].duty);
                Send_Hex(Lev[address].hits);
			}
            Send_Hex(OpenTimes);
			Send_Hex(0x00);//补位
            RCIE=1;
		}break;
        //Read All Data
		case 'O':
		{
            RCIE=0;
			for(address=0; address<5; address++)
			{
                Send_Hex(Lev[address].duty);
                Send_Hex(Lev[address].hits);
			}
            Send_Hex(OpenTimes);
			Send_Hex(0x00);Send_Hex(0x00);Send_Hex(0x00);Send_Hex(0x00);Send_Hex(0x00);//补位
            
            int i=0;
			for(i=0;i<254;i++)
			{
                SPEN=0;RCIE=0;CREN=0;
                FRAM_INIT();
                for(address=8*i+16; address<8*i+24; address++)
                {
                    if(i==30)
                    {
                        AllData[address-8*i+16]=read_F(address+1792);
                    }
                    else
                    {
                        AllData[address-8*i+16]=read_F(address);
                    }
                }
                SSPEN=0;RCIE=1;
                UART_INIT();
                RCIE=1;
                for(address=8*i+16; address<8*i+24; address++)
                {
                    Send_Hex(AllData[address-8*i+16]);
                }
			}
            RCIE=1;
		} break;
        //Set Default Parameters
		case 'S':
        {
            SPEN=0;RCIE=0;CREN=0;
            FRAM_INIT();
			SetDefaultPara();
            SSPEN=0;RCIE=1;
            UART_INIT();
            RCIE=1;
        }break;
        //Set ALL Data=0x00
        case 'F':
        {
            SPEN=0;RCIE=0;CREN=0;
            FRAM_INIT();
			DataClean(0x10,0x810);
            SSPEN=0;RCIE=1;
            UART_INIT();
            RCIE=1;
        }break;
        //any other key to redispaly the menu
		default:
        {
			Display_Menu();
        }break;
		}
	}
    unsigned char change=(PORTA >>2) & 0x07;
    if(IC1IF==1)//转速计算
    {
        IC1IF=0;
        if(Page==2)
        {
            if(MOTOR_BUTTON==1 && OverTEMP==0)
            {
                if(MOTOR_DIR==0)
                {
                    OVDCOND=STATE_HALL[change-0x01];
                }
                else
                {
                    OVDCOND=STATE_HALL_BACK[change-0x01];
                }
                //Speed=(2500000.0/(CAP1BUFL+CAP1BUFH*256.0));
                SL=CAP1BUFL;
                SH=CAP1BUFH;
                //LCD_P8x16Str(16,0,rate);
            }
            //CAP1REN=1;
            //CAP1REN=0;
        }
    }
    if(IC2QEIF==1)
    {
        IC2QEIF=0;
        if(Page==2)
        {
            if(MOTOR_BUTTON==1 && OverTEMP==0)
            {
                if(MOTOR_DIR==0)
                {
                    OVDCOND=STATE_HALL[change-0x01];
                }
                else
                {
                    OVDCOND=STATE_HALL_BACK[change-0x01];
                }
            }
        }
        //CAP2REN=1;
        //CAP2REN=0;
    }
    if(IC3DRIF==1)
    {
        IC3DRIF=0;
        if(MOTOR_BUTTON==1 && OverTEMP==0)
        {
            if(MOTOR_BUTTON==1)
            {
                if(MOTOR_DIR==0)
                {
                    OVDCOND=STATE_HALL[change-0x01];
                }
                else
                {
                    OVDCOND=STATE_HALL_BACK[change-0x01];
                }
            }
        }
        //CAP3REN=1;
        //CAP3REN=0;
    }
    if(TMR5IF==1)
    {
        TMR5IF=0;
    }
    /*
    if(TMR0IF==1)
    {
        TMR0IF=0;
        //Show_Flag=1;
        //GetTemp();
        Speed=(625000.0/(SL+SH*256.0));
    }*/
    /*
    if(ADIF==1)//AD中断
    {  
        if(Page==2 )
        {
            AD_RES=1.25*(ADRESH & 0x03)+0.0048*ADRESL;//计算AD结果
        }
        ADIF=0; 
    }  */
}


/*
void interrupt CAPTURE( void )
{   //A is highside,B is low side

    if(IC1IF==1)
    {
        if(MOTOR_BUTTON==START_MOTOR)
        {
            change=(PORTA >>2) & 0x07;
            OVDCOND=STATE_HALL[change-0x01];

            CAPH=CAP1BUFH;
            CAPL=CAP1BUFL;
            //LCD_P8x16Str(16,0,rate);

        }
        TMR5H=0x00;
        TMR5L=0x00;
        CAP1REN=1;
        CAP1REN=0;
        IC1IF=0;
    }
    if(IC2QEIF==1)
    {
        if(MOTOR_BUTTON==START_MOTOR)
        {
            change=(PORTA >>2) & 0x07;
            OVDCOND=STATE_HALL[change-0x01];

            unsigned speed=1000000.0/6.0/(double)(CAP1BUFH*256.0+CAP1BUFL);
            itoa(rate,speed,10);
            LCD_P8x16Str(16,2,rate);
        }
        TMR5H=0x00;
        TMR5L=0x00;
        CAP2REN=1;
        CAP1REN=0;
        IC2QEIF=0;
    }
    if(IC3DRIF==1)
    {
        if(MOTOR_BUTTON==START_MOTOR)
        {
            change=(PORTA >>2) & 0x07;
            OVDCOND=STATE_HALL[change-0x01];

            //unsigned speed=1000000.0/6.0/(double)(CAP1BUFH*256.0+CAP1BUFL);
            //itoa(rate,speed,10);
            //LCD_P8x16Str(16,4,rate);
        }
        TMR5H=0x00;
        TMR5L=0x00;
        CAP3REN=1;
        CAP1REN=0;
        IC3DRIF=0;
    }

    if(TMR5IF==1)
    {
        //TMR5H=0x00;
        //TMR5L=0x00;
        if(MOTOR_BUTTON==START_MOTOR)
        {
        change=(PORTA >>2) & 0x07;
        OVDCOND=STATE_HALL[change-0x01];
        }
        TMR5IF=0;
    }
}
*/
/*
void interrupt UART11( void )
{
    if(RCIF==1)
    {
        switch (RCREG)
        {
            case 'R':
            {
                int address;
                Send_Hex(RAM_Data[11]);
                for(address=0x01;address<0x0b;address++)
                {
                    Send_Hex(RAM_Data[address]);
                }
                Send_Hex(0x00);
            }break;

            case 'O':
            {
                int address;
                Send_Hex(RAM_Data[11]);
                for(address=0x01;address<0x0b;address++)
                {
                    Send_Hex(RAM_Data[address]);
                }
                Send_Hex(0x00);Send_Hex(0x00);Send_Hex(0x00);Send_Hex(0x00);Send_Hex(0x00);
                //unsigned char AllData[108];
                SPEN=0;
                FRAM_INIT();
                for(address=0x10;address<0x60;address++)
                {
                    //AllData[address-0x10]=read_F(address);
                }
                SPEN=1;
            }break;
            case 'S':SetDefaultPara();break;
            case 'F':DataClean(0x10,0x3ff);break;

            default:Send_Hex(0xff);break;
        }
        RCIF=0;
    }
}
*/


