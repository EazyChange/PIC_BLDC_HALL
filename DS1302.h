/* 
 * File:   DS1302.h
 * Author: Administrator
 *
 * Created on 2016年2月19日, 下午7:26
 */

#ifndef DS1302_H
#define	DS1302_H

//#define _XTAL_FREQ 4000000
//DS1302引脚定义
#define IO   RE0
#define CLK RD4
#define RST  RE2
//DS1302端口数据读写(方向)
#define SET_IO_RD() TRISE0 = 1
#define SET_IO_WR() TRISE0 = 0

//volatile unsigned char rec ;
unsigned char table[7]={0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x16};
/* 起始时间：               秒    分   时    日    月   星期   年   */
unsigned char start[7]={0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x16};//设置初始时间
//DS1302相关函数
void INIT_PORT(void);
void DS1302_INIT(void);
void set_time(unsigned char *date);
void get_time(void);
void write_cmd(unsigned char cmd);//写命令
void write_data(unsigned char data);//写数据
unsigned char read_time(void);
//void delayms(int n);
//void Ascii2Str(unsigned char ascii,unsigned char buf[],int weigh);
void Ascii2Str(unsigned char ascii,unsigned char buf[]);
//int Hex2Str(const unsigned char hexh,const unsigned char hexl, char buf[]);
//int GetBits(const unsigned ui);
//void TimeFormat( char *ascii,  char min,  char max,int mode);
void TimeFormat( char *ascii,  char min,  char max);
unsigned char Hex2Dex(unsigned char Hex);
#endif	/* DS1302_H */

