/*128*64在列方向分为8页，每页为8个像素。*/

#include <pic18f4431.h>
#include <stdlib.h>
#include "SPI.h"
#include "LCD.h"
#include "motor.h"
#include "DS1302.h"
#include "DS18B20.h"
#include "AD.h"
#include <stdio.h>

#define LCD_SCL RC5  
//#define LCD_SDA RC7
#define LCD_CS  RD2
#define LCD_RST RD1
#define LCD_DC  RD0 

//#define XLevelL		0x00
//#define XLevelH		0x10
//#define XLevel		((XLevelH&0x0F)*16+XLevelL)
//#define Max_Column	128
//#define Max_Row		64
//#define	Brightness	0xCF 

//#define X_WIDTH 128
//#define Y_WIDTH 64
//======================================



void LCD_WrDat(unsigned char dat)
{
    LCD_CS=0;
    LCD_DC=1;
    LCD_SCL=0;
    write_spi(dat);
    LCD_CS=1;
}
void LCD_WrCmd(unsigned char cmd)
{
    LCD_CS=0;
	LCD_DC=0;
    LCD_SCL=0;
    write_spi(cmd);
    LCD_CS=1;
}
void LCD_Set_Pos(unsigned char x, unsigned char y)
{ 
  LCD_WrCmd(0xb0+y);
  LCD_WrCmd(((x&0xf0)>>4)|0x10);
  LCD_WrCmd((x&0x0f)|0x00); 
} 
/*
void LCD_Fill(unsigned char bmp_dat)
{
	unsigned char y,x;
	
	for(y=0;y<8;y++)
	{
		LCD_WrCmd(0xb0+y);
		LCD_WrCmd(0x01);
		LCD_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
			LCD_WrDat(bmp_dat);
	}
}
 */
void LCD_CLS(void)
{
	unsigned char y,x;	
	for(y=0;y<8;y++)
	{
		LCD_WrCmd(0xb0+y);
		LCD_WrCmd(0x01);
		LCD_WrCmd(0x10); 
		for(x=0;x<128;x++)
			LCD_WrDat(0);
	}
}
/*
void LCD_DLY_ms(unsigned int ms)
{                         
  unsigned int a;
  while(ms)
  {
    a=1335;
    while(a--);
    ms--;
  }
  return;
} 
 */
/*
void itoa_1(unsigned char buf[],int data,char bits)
{
    if(bits==1)
    {
        buf[0]=data + 0x30;
        buf[1]='\0';
        return;
    }
    if(bits==2)
    {
        buf[0]=data / 10 + 0x30;
        buf[1]=data % 10 +0x30;
        buf[2]='\0';
        return;
    }
    if(bits==3)
    {
        buf[0]=data / 100 + 0x30;
        buf[1]=data % 100 / 10 + 0x30;
        buf[2]=data % 10 + 0x30;
        buf[3]='\0';
        return;
    }
    if(bits==3)
    {
        buf[0]=data / 1000 + 0x30;
        buf[1]=data % 1000 / 100 + 0x30;
        buf[2]=data % 1000 % 100 /10 + 0x30;
        buf[3]=data % 10 + 0x30;
        buf[4]='\0';
        return;
    }
}
*/
void LCD_INIT(void)        
{  
    INIT_SPI();
    TRISD2=0;TRISD1=0;TRISD0=0;
    LCD_RST=0;
    delay(500);
	LCD_RST=1;
	//从上电到下面开始初始化要有足够的时间，即等待RC复位完毕   
    LCD_CS=0;
  LCD_WrCmd(0xae);//--turn off oled panel
  LCD_WrCmd(0x00);//---set low column address
  LCD_WrCmd(0x10);//---set high column address
  LCD_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
  LCD_WrCmd(0x81);//--set contrast control register
  LCD_WrCmd(0xcf); // Set SEG Output Current Brightness
  LCD_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
  LCD_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
  LCD_WrCmd(0xa6);//--set normal display
  LCD_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
  LCD_WrCmd(0x3f);//--1/64 duty
  LCD_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
  LCD_WrCmd(0x00);//-not offset
  LCD_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
  LCD_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
  LCD_WrCmd(0xd9);//--set pre-charge period
  LCD_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  LCD_WrCmd(0xda);//--set com pins hardware configuration
  LCD_WrCmd(0x12);
  LCD_WrCmd(0xdb);//--set vcomh
  LCD_WrCmd(0x40);//Set VCOM Deselect Level
  LCD_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
  LCD_WrCmd(0x02);//
  LCD_WrCmd(0x8d);//--set Charge Pump enable/disable
  LCD_WrCmd(0x14);//--set(0x10) disable
  LCD_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
  LCD_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
  LCD_WrCmd(0xaf);//--turn on oled panel
  //LCD_Fill(0x00);  //初始清屏
  LCD_CLS();
  LCD_Set_Pos(0,0); 	
} 
/*
//==============================================================
//函数名：LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char *p)
//功能描述：写入一组标准ASCII字符串
//参数：显示的位置（x,y），y为页范围0～7，要显示的字符串
//返回：无
//==============================================================  
void LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[])
{
  unsigned char c=0,i=0,j=0;      
  while (ch[j]!='\0')
  {    
    c =ch[j]-32;
    if(x>126){x=0;y++;}
    LCD_Set_Pos(x,y);    
  	for(i=0;i<6;i++)     
  	  LCD_WrDat(F6x8[c][i]);  
  	x+=6;
  	j++;
  }
}
*/

void show6x8(unsigned char x,unsigned char y,unsigned char c)
{
    unsigned char i=0,j=0;
    if(x>126){x=0;y++;}
    LCD_Set_Pos(x,y);    
  	for(i=0;i<6;i++)     
  	  LCD_WrDat(F6x8_1[c][i]);  
  	x+=6;
  	j++;
}

void LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char mode)
{
    if(mode==0)
    {
        show6x8(x,y,0);show6x8(x+6,y,3);show6x8(x+12,y,3);show6x8(x+18,y,5);
        show6x8(x+24,y,7);
        show6x8(x+30,y,2);show6x8(x+36,y,3);show6x8(x+42,y,4);show6x8(x+48,y,2);show6x8(x+54,y,1);show6x8(x+60,y,0);show6x8(x+66,y,4);
    }
    else
    {
        show6x8(x,y,16);show6x8(x+6,y,16);show6x8(x+12,y,16);show6x8(x+18,y,6);
        show6x8(x+24,y,10);show6x8(x+30,y,14);show6x8(x+36,y,14);show6x8(x+42,y,14);show6x8(x+48,y,15);show6x8(x+54,y,8);show6x8(x+60,y,13);show6x8(x+66,y,6);
        show6x8(x+72,y,9);show6x8(x+78,y,12);show6x8(x+84,y,11);
    }
}

//==============================================================
//函数名：LCD_P8x16Str(unsigned char x,unsigned char y,unsigned char *p)
//功能描述：写入一组标准ASCII字符串
//参数：显示的位置（x,y），y为页范围0～7，要显示的字符串
//返回：无
//============================================================== 
void LCD_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[])
{
  unsigned char c=0,i=0,j=0; 
  while (ch[j]!='\0')
  {    
    c =ch[j]-32;
    if(x>120){x=0;y++;}
    LCD_Set_Pos(x,y);    
  	for(i=0;i<8;i++)     
  	  LCD_WrDat(F8X16[c*16+i]);
  	LCD_Set_Pos(x,y+1);    
  	for(i=0;i<8;i++)     
  	  LCD_WrDat(F8X16[c*16+i+8]);  
  	x+=8;
  	j++;
  }
}
void LCD_P8x16Sp(unsigned char x,unsigned char y,unsigned char ch)
{
    unsigned char c=0,i=0;
    switch (ch)
    {
        case 'V':c=31;break;
        case 'r':c=32;break;
        case 'm':c=33;break;
        case 'O':c=34;break;
        case 'K':c=35;break;
        case '_':c=36;break;
    }
    LCD_Set_Pos(x,y);    
  	for(i=0;i<8;i++)     
  	  LCD_WrDat(F8X16[c*16+i]);
  	LCD_Set_Pos(x,y+1);    
  	for(i=0;i<8;i++)     
  	  LCD_WrDat(F8X16[c*16+i+8]);  
}
/*
void LCD_P8x16Num(unsigned char x,unsigned char y,int num)
{
  unsigned char c=0,i=0,j=0;
  unsigned char ch[5];
  itoa(ch,num,10);
  while (ch[j]!='\0')
  {    
    c =ch[j]-32;
    if(x>120){x=0;y++;}
    LCD_Set_Pos(x,y);    
  	for(i=0;i<8;i++)     
  	  LCD_WrDat(F8X16[c*16+i]);
  	LCD_Set_Pos(x,y+1);    
  	for(i=0;i<8;i++)     
  	  LCD_WrDat(F8X16[c*16+i+8]);  
  	x+=8;
  	j++;
  }  
}
 */
//输出16*16汉字字符串
void LCD_HZ(unsigned char x,unsigned char y,unsigned char* data,unsigned char N)
{
	unsigned char wm=0;
	unsigned int adder=32*N;    	
  	LCD_Set_Pos(x , y);
  	for(wm = 0;wm < 16;wm++)               
  	{
  		LCD_WrDat(data[adder]);	
  		adder += 1;
  	}      
  	LCD_Set_Pos(x,y + 1); 
  	for(wm = 0;wm < 16;wm++)          
  	{
  		LCD_WrDat(data[adder]);
  		adder += 1;
  	} 	  	
}
/*
//输出8*16符号，办字符
void LCD_HF(unsigned char x,unsigned char y,unsigned char* data,unsigned char N)
{
	unsigned char wm=0;
	unsigned int adder=16*N;    	
  	LCD_Set_Pos(x , y);
  	for(wm = 0;wm < 8;wm++)               
  	{
  		LCD_WrDat(data[adder]);	
  		adder += 1;
  	}      
  	LCD_Set_Pos(x,y + 1); 
  	for(wm = 0;wm < 8;wm++)          
  	{
  		LCD_WrDat(data[adder]);
  		adder += 1;
  	} 	  	
}
 */
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/

void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		LCD_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	LCD_WrDat(BMP[j++]);	    	
	    }
	}
}
/*箭头移动*/
//x=0表示第二行，1表示第三行，2表示第四行
/*
void LCD_Arrow_Change(int x,int pos)
{
    //LCD_P8x16Str(pos,((x-1)%3+1)*2,"  ");
    LCD_P8x16Str(pos,(x%3+1)*2,"->");
}*/
/*
void LCD_CLS_PART(unsigned x,unsigned y)
{
    unsigned char wm=0;
	unsigned int adder=0;    	
  	LCD_Set_Pos(x , y);
  	for(wm = 0;wm < 14;wm++)               
  	{
  		LCD_WrDat(Blank[adder]);	
  		adder += 1;
  	}      
  	LCD_Set_Pos(x,y + 1); 
  	for(wm = 0;wm < 14;wm++)          
  	{
  		LCD_WrDat(Blank[adder]);
  		adder += 1;
  	} 	  	
}
 */
//初始界面持续2秒

void DispLogo()
{
    OLED_DrawBMP(0,0,128,4,Logo);//Logo
    LCD_HZ(20,4,F16x16_HZ,31);//电
	LCD_HZ(44,4,F16x16_HZ,61);//动
	LCD_HZ(68,4,F16x16_HZ,62);//扳
	LCD_HZ(92,4,F16x16_HZ,63);//手
    LCD_P6x8Str(28,6,0);
    LCD_P6x8Str(17,7,1);    
    //LCD_P6x8Str(28,6,"0559-3583108");
    //LCD_P6x8Str(17,7,"www.hssstar.com");
}

//运行参数设置界面
extern struct MotorPara;
void SetPage(struct MotorPara mp,int lev, char x)
{
    LCD_P8x16Str(16,2,"  ");LCD_P8x16Str(16,4,"  ");LCD_P8x16Str(16,6,"  ");
    LCD_P8x16Str(16,(x%3+1)*2,"->");
    
    unsigned char buf[4];
    //1st 运行参数设置
    LCD_HZ(32,0,F16x16_HZ,26);LCD_HZ(48,0,F16x16_HZ,27);LCD_HZ(64,0,F16x16_HZ,28);
    LCD_HZ(80,0,F16x16_HZ,29);LCD_HZ(96,0,F16x16_HZ,42);LCD_HZ(112,0,F16x16_HZ,43);
    //2nd 档位1-5
    LCD_HZ(32,2,F16x16_HZ,12);LCD_HZ(48,2,F16x16_HZ,13);LCD_P8x16Str(64,2,":");
    itoa(buf,lev,10);
    //sprintf(buf,"%d",lev);
    LCD_P8x16Str(80,2,buf);
    //3rd 占空比1-100
    LCD_HZ(32,4,F16x16_HZ,44);LCD_HZ(48,4,F16x16_HZ,45);LCD_HZ(64,4,F16x16_HZ,46);LCD_P8x16Str(80,4,":");
    itoa(buf,mp.duty,10);
    //sprintf(buf,"%d",mp.duty);
    LCD_P8x16Str(96,4,buf);if(mp.duty<10){LCD_P8x16Str(104,4,"  ");}if(mp.duty<100){LCD_P8x16Str(112,4," ");}
    //4th 打击次数0-100
    LCD_HZ(32,6,F16x16_HZ,14);LCD_HZ(48,6,F16x16_HZ,15);LCD_HZ(64,6,F16x16_HZ,16);LCD_HZ(80,6,F16x16_HZ,17);LCD_P8x16Str(96,6,":");
    itoa(buf,mp.hits,10);
    //sprintf(buf,"%d",mp.hits);
    LCD_P8x16Str(104,6,buf);if(mp.hits<10){LCD_P8x16Str(112,6,"  ");}if(mp.hits<100){LCD_P8x16Str(120,6," ");}
}
/*
void TempPage(void)
{
    unsigned char buf[5];
	LCD_HZ(8, 0, F16x16_HZ,50);//" "

	LCD_HZ(24, 0, F16x16_HZ,26);//断
	LCD_HZ(40, 0, F16x16_HZ,27);//电
	LCD_HZ(56, 0, F16x16_HZ,28);//温
	LCD_HZ(72, 0, F16x16_HZ,29);//度
	LCD_HZ(88, 0, F16x16_HZ,38);//设
	LCD_HZ(104, 0, F16x16_HZ,39);//置

	LCD_HF(8, 2, F8x16_FH, 1);LCD_HF(16, 2, F8x16_FH, 2);//->
	LCD_HZ(24, 2, F16x16_HZ,28);//编
	LCD_HZ(40, 2, F16x16_HZ,29);//号
	LCD_HF(56, 2, F8x16_FH,0);//:
	itoa(buf,123,10);LCD_P8x16Str(72, 2, buf);
}
 */
void TimePage(unsigned char year,unsigned char month,unsigned char day,unsigned char hour,unsigned char min,unsigned char sec,char x)
{
    unsigned char buf[3];
    
	LCD_HZ(16, 0, F16x16_HZ,34);//日
	LCD_HZ(32, 0, F16x16_HZ,35);//期
	LCD_HZ(48, 0, F16x16_HZ,36);//时
	LCD_HZ(64, 0, F16x16_HZ,37);//间
	LCD_HZ(80, 0, F16x16_HZ,42);//设
	LCD_HZ(96, 0, F16x16_HZ,43);//置
   
    LCD_P8x16Str(8,2,"20");
    Ascii2Str(year,buf);
    LCD_P8x16Str(24,2,buf);
    LCD_HZ(40, 2, F16x16_HZ,6);//年
    Ascii2Str(month,buf);
    LCD_P8x16Str(56,2,buf);
    LCD_HZ(72, 2, F16x16_HZ,7);//月
    Ascii2Str(day,buf);
    LCD_P8x16Str(88,2,buf);
    LCD_HZ(104, 2, F16x16_HZ,8);//日
    
    Ascii2Str(hour,buf);
    LCD_P8x16Str(28, 4, buf);//时
    LCD_P8x16Str(44,4,":");
    Ascii2Str(min,buf);
    LCD_P8x16Str(52, 4, buf);//分
    LCD_P8x16Str(68,4,":");
    Ascii2Str(sec,buf);
    LCD_P8x16Str(76, 4, buf);//秒
    
    if(x<=2)
    {
        LCD_P8x16Sp(32+x*32,2,'_');
    }
    else
    {
        LCD_P8x16Sp(36+(x-3)*24,4,'_');
    }
    //delay(300);
/*    
    switch (x)
    {
        case 0:LCD_P8x16Sp(24,2,'_');LCD_P8x16Sp(32,2,'_');break;
        case 1:LCD_P8x16Sp(56,2,'_');LCD_P8x16Sp(64,2,'_');break;
        case 2:LCD_P8x16Sp(88,2,'_');LCD_P8x16Sp(96,2,'_');break;
        case 3:LCD_P8x16Sp(28,4,'_');LCD_P8x16Sp(36,4,'_');break;
        case 4:LCD_P8x16Sp(52,4,'_');LCD_P8x16Sp(60,4,'_');break;
        case 5:LCD_P8x16Sp(76,4,'_');LCD_P8x16Sp(84,4,'_');break;
        default:break;
    } */
}
extern unsigned char table[7];//DS1302的输出时间数组，分别为 秒  分  时  日  月  星期  年
extern char OperatorID;//操作人员编号
extern char OpenTimes;//开机次数
//操作编号选择界面
void IDPage(void)
{
    unsigned char buf[4];

	LCD_HZ(24, 0, F16x16_HZ,0);//操
	LCD_HZ(40, 0, F16x16_HZ,1);//作
	LCD_HZ(56, 0, F16x16_HZ,2);//编
	LCD_HZ(72, 0, F16x16_HZ,3);//号
	LCD_HZ(88, 0, F16x16_HZ,4);//选
	LCD_HZ(104, 0, F16x16_HZ,5);//择

	LCD_P8x16Str(8, 2, "->");//->
	LCD_HZ(24, 2, F16x16_HZ,2);//编
	LCD_HZ(40, 2, F16x16_HZ,3);//号
	LCD_P8x16Str(56, 2, ":");//:
    
    itoa(buf,OperatorID,10);
    if(OperatorID<=9){LCD_P8x16Str(72,2," ");}LCD_P8x16Str(64,2,buf);
    itoa(buf,OpenTimes,10);
    //if(OpenTimes<=9){LCD_P8x16Str(104,2," ");}
    LCD_P8x16Str(96,2,buf);

    Ascii2Str(table[6],buf);LCD_P8x16Str(24, 4, buf);LCD_P8x16Str(8, 4, "20");LCD_HZ(40, 4, F16x16_HZ,6);//年
    Ascii2Str(table[4],buf);LCD_P8x16Str(56, 4, buf);LCD_HZ(72, 4, F16x16_HZ,7);//月
	Ascii2Str(table[3],buf);LCD_P8x16Str(88, 4, buf);LCD_HZ(104, 4, F16x16_HZ,8);//日

	Ascii2Str(table[2],buf);LCD_P8x16Str(30, 6, buf);  LCD_P8x16Str(46, 6, ":"); 
    Ascii2Str(table[1],buf);LCD_P8x16Str(54, 6, buf); LCD_P8x16Str(70, 6, ":");
    Ascii2Str(table[0],buf);LCD_P8x16Str(76, 6, buf); //时:分:秒 
	//LCD_HZ(88, 6, F16x16_HZ,9);itoa(buf,2,10);LCD_P8x16Str(104, 6, buf);//周
}
//操作界面
//extern unsigned char temp_int,temp_df;//DS18B20温度参数
int time=0;
extern bit MOTOR_BUTTON;
extern bit MOTOR_DIR;
extern bit OverTEMP;//过温
extern short Speed;
extern float AD_RES;
extern float TEMP_VALUE;
extern unsigned char hits;
void DispPage1(struct MotorPara mp,int lev)
{
    unsigned char buf[5];
    //第一行
    itoa(buf,lev,10);
    LCD_P8x16Str(8,0,buf);
    LCD_HZ(16,0,F16x16_HZ,12);//档
    itoa(buf,mp.duty,10);
    if(mp.duty>=100)
    {
        LCD_P8x16Str(40,0,buf);
    }
    else
    {
        LCD_P8x16Str(40,0," ");
        LCD_P8x16Str(48,0,buf);
    }
    LCD_P8x16Str(64,0,"%");
    itoa(buf,mp.hits,10);
    if(mp.hits>=100)
    {
        LCD_P8x16Str(80,0,buf);
    }
    else
    {
        LCD_P8x16Str(80,0," ");
        LCD_P8x16Str(88,0,buf);  
    }
    itoa(buf,hits,10);
    LCD_P8x16Str(88,2,buf);
	LCD_HZ(104,0,F16x16_HZ,16);//次
    //第二行
    if(time>=100)
    {
        LCD_P8x16Str(16,2,"    ");
        itoa(buf,Speed,10);
        LCD_P8x16Str(16,2,buf);
    }
    LCD_P8x16Sp(48,2,'r');LCD_P8x16Str(56,2,"/");LCD_P8x16Sp(64,2,'m');
    LCD_HZ(104,2,F16x16_HZ,16);//次
    //第三行
  /*  itoa(buf,temp_int,10);
    LCD_P8x16Str(16,4,buf);
    LCD_P8x16Str(32,4,".");
    itoa(buf,temp_df,10);
    LCD_P8x16Str(40,4,buf);*/
    if(time>=100)
    {
        itoa(buf,TEMP_VALUE,10);
        LCD_P8x16Str(16,4,buf);
        time=0;
    }
    time++;
    //LCD_P8x16Str(32,4,".");
    //itoa(buf,(int)(TEMP_VALUE*10)%10,10);
    //LCD_P8x16Str(40,4,buf);
    LCD_HZ(48, 4, F16x16_HZ,33);//度
    
    itoa(buf,AD_RES,10);
    //if(AD_RES>=10.0)
    {
        //LCD_P8x16Str(72,4,buf);
    }
    //else
    {
        LCD_P8x16Str(80,4,buf);
    }
    itoa(buf,(int)(AD_RES*10)%10,10);
    LCD_P8x16Str(96,4,buf);
    LCD_P8x16Str(88,4,".");
    LCD_P8x16Sp(104,4,'V');
    //第四行
    if(TEMP_VALUE>=80)
    {
        OverTEMP=1;
    }//不会有把OverTEMP置0的函数，只有重新开机
    MOTOR_BUTTON==1?LCD_HZ(0, 6, F16x16_HZ,24):LCD_HZ(0, 6, F16x16_HZ,25);
    MOTOR_DIR==0?LCD_HZ(112, 6, F16x16_HZ,59):LCD_HZ(112, 6, F16x16_HZ,60);
    if(OverTEMP==1)//过温
    {
        /*
        LCD_HZ(40,6,F16x16_HZ,18);//温
        LCD_HZ(56,6,F16x16_HZ,29);//度
        LCD_HZ(72,6,F16x16_HZ,64);//高
         */
        LCD_HZ(40,6,F16x16_HZ,64);//高
        LCD_HZ(56,6,F16x16_HZ,18);//温
        MOTOR_BUTTON=0;
    }
}
void SetOKPage()
{
    LCD_HZ(24,2,F16x16_HZ,28);//参
    LCD_HZ(40,2,F16x16_HZ,29);//数
    LCD_HZ(56,2,F16x16_HZ,42);//设
    LCD_HZ(72,2,F16x16_HZ,43);//置
    LCD_P8x16Sp(88,2,'O');//O
    LCD_P8x16Sp(96,2,'K');//K
    //LCD_P8x16Str(104,2,"!");//!
}
void ResetPage(char x)
{
    LCD_P8x16Str(0,2,"  ");LCD_P8x16Str(0,4,"  ");LCD_P8x16Str(0,6,"  ");
    //LCD_Arrow_Change(x,0);
    LCD_P8x16Str(0,(x%3+1)*2,"->");
    
	LCD_HZ(32, 0, F16x16_HZ,40);//出
	LCD_HZ(48, 0, F16x16_HZ,41);//厂
	LCD_HZ(64, 0, F16x16_HZ,42);//设
	LCD_HZ(80, 0, F16x16_HZ,43);//置
    
    LCD_HZ(16, 2, F16x16_HZ,51);//默
	LCD_HZ(32, 2, F16x16_HZ,52);//认
    LCD_HZ(48, 2, F16x16_HZ,28);//参
    LCD_HZ(64, 2, F16x16_HZ,29);//数
    LCD_HZ(80, 2, F16x16_HZ,42);//设
    LCD_HZ(96, 2, F16x16_HZ,43);//置
    
    LCD_HZ(16, 4, F16x16_HZ,51);//默
	LCD_HZ(32, 4, F16x16_HZ,52);//认
    LCD_HZ(48, 4, F16x16_HZ,36);//时
    LCD_HZ(64, 4, F16x16_HZ,37);//间
    LCD_HZ(80, 4, F16x16_HZ,42);//设
    LCD_HZ(96, 4, F16x16_HZ,43);//置

    LCD_HZ(16, 6, F16x16_HZ,53);//清
	LCD_HZ(32, 6, F16x16_HZ,54);//空
    LCD_HZ(48, 6, F16x16_HZ,55);//所
	LCD_HZ(64, 6, F16x16_HZ,56);//有
	LCD_HZ(80, 6, F16x16_HZ,57);//数
	LCD_HZ(96, 6, F16x16_HZ,58);//据   
}
