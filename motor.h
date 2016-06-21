/* 
 * File:   motor.h
 * Author: Administrator
 *
 * Created on 2016年1月18日, 下午1:22
 */

#ifndef MOTOR_H
#define	MOTOR_H

#define NOP() asm("nop");

#define EN_GATE RE1
//#define CLK_DRV RC5
//#define DI_DRV  RC7
//#define DO_DRV  RC4
//#define CS_DRV  RA5



bit MOTOR_BUTTON = 0;
bit MOTOR_DIR = 0;
short Speed=0;
unsigned char hits=0;

struct MotorPara{
    char duty;
    char hits;
}Lev[5];

struct MotorPara SetPara[5];
//unsigned char RAM_Data[16];
unsigned char AllData[8];
unsigned char UseNum=0;

void delay(unsigned int t);	 
void delay_ms(unsigned int t);
void delay_n(unsigned char n);

void Display_Menu();
unsigned char Read_Parameters();
void SetDefaultPara();
void DRV_INIT();
//void itoa_1(unsigned char buf[], unsigned char data, unsigned char i);

#endif	/* MOTOR_H */

