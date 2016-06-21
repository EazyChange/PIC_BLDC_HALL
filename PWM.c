#include <pic18f4431.h>
#include "Delays.h"
//#include <stdio.h>
#include "LCD.h"
#include "PWM.h"

#define NOP() asm("Nop")

/*************设置的周期和占空比****************************/
unsigned char periodH=0x00; //23.4kHz
unsigned char periodL=0x55; //23.4kHz
unsigned char dutyH;
unsigned char dutyL;
/*
void PWM_HALL(unsigned char i)
{
        switch(i)
        {
            case 0x04:OVDCOND=0x42;break;
            case 0x06:OVDCOND=0x06;break;
            case 0x02:OVDCOND=0x84;break;
            case 0x03:OVDCOND=0x81;break;
            case 0x01:OVDCOND=0x09;break;
            case 0x05:OVDCOND=0x48;break;
            default:OVDCOND=0x00;break;
        }    
}*/
//捕获初始化
void CAP_INIT()
{
    TRISA2=1;TRISA3=1;TRISA4=1;//RA2-4设置为输入
    ANSEL0 &= 0xe3;//AD2-4关闭，并设置为IO口
    T5CON=0xdd;//1101 1101
    TMR5IF=0;//中断标志位清零
    TMR5IE=1;//中断使能
    IC1IE=1;
    IC2QEIE=1;
    IC3DRIE=1;
    IC1IF=0;
    IC2QEIF=0;
    IC3DRIF=0;
    CAP1CON=0x48;//状态改变时捕获
    CAP2CON=0x48;
    CAP3CON=0x48;
    RESEN=0;
}
/******设置占空比，n为0-100的整数对应0%-100%**************************/
void SET_DUTY(unsigned char n)
{
    int temp = (periodH * 256 + periodL) * 4.0 * n / 100.0;
	dutyH = temp >> 8;
	dutyL = temp & 0xff;
    
    PDC0H=dutyH;//确定占空比
    PDC0L=dutyL;//80 100%
    PDC1H=dutyH;//确定占空比
    PDC1L=dutyL;
    PDC2H=dutyH;//确定占空比
    PDC2L=dutyL;
    //PDC3H=dutyH;//确定占空比
    //PDC3L=dutyL;
}
//PWM初始化
void PWM_INIT()
{
    TRISB0=0;TRISB1=0;TRISB2=0;TRISB3=0;TRISD5=0;TRISB4=0;

    PTCON0=0x00;//配置单步模式     
    PWMCON0=0x4f;//01011111
    PWMCON1=0x00; 
    OVDCOND=0x00;
    
    PTPERH=periodH;//确定PWM周期
    PTPERL=periodL;  //4M 4KHz 0x20 must be 60.68K 
    //SET_DUTY(60);
    /*把PDC右移两位，如果等于PTPER，则占空比为100%*/
    /*
    PDC0H=dutyH;//确定占空比
    PDC0L=dutyL;//80 100%
    PDC1H=dutyH;//确定占空比
    PDC1L=dutyL;
    PDC3H=dutyH;//确定占空比
    PDC3L=dutyL;
    */
    PTMRL=0;
    PTMRH=0;
    PTEN=1;//PWM使能,时基向上计数
}


