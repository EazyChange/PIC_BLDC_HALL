#include <pic18f4431.h>
#include "Delays.h"
//#include <stdio.h>
#include "LCD.h"
#include "PWM.h"

#define NOP() asm("Nop")

/*************���õ����ں�ռ�ձ�****************************/
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
//�����ʼ��
void CAP_INIT()
{
    TRISA2=1;TRISA3=1;TRISA4=1;//RA2-4����Ϊ����
    ANSEL0 &= 0xe3;//AD2-4�رգ�������ΪIO��
    T5CON=0xdd;//1101 1101
    TMR5IF=0;//�жϱ�־λ����
    TMR5IE=1;//�ж�ʹ��
    IC1IE=1;
    IC2QEIE=1;
    IC3DRIE=1;
    IC1IF=0;
    IC2QEIF=0;
    IC3DRIF=0;
    CAP1CON=0x48;//״̬�ı�ʱ����
    CAP2CON=0x48;
    CAP3CON=0x48;
    RESEN=0;
}
/******����ռ�ձȣ�nΪ0-100��������Ӧ0%-100%**************************/
void SET_DUTY(unsigned char n)
{
    int temp = (periodH * 256 + periodL) * 4.0 * n / 100.0;
	dutyH = temp >> 8;
	dutyL = temp & 0xff;
    
    PDC0H=dutyH;//ȷ��ռ�ձ�
    PDC0L=dutyL;//80 100%
    PDC1H=dutyH;//ȷ��ռ�ձ�
    PDC1L=dutyL;
    PDC2H=dutyH;//ȷ��ռ�ձ�
    PDC2L=dutyL;
    //PDC3H=dutyH;//ȷ��ռ�ձ�
    //PDC3L=dutyL;
}
//PWM��ʼ��
void PWM_INIT()
{
    TRISB0=0;TRISB1=0;TRISB2=0;TRISB3=0;TRISD5=0;TRISB4=0;

    PTCON0=0x00;//���õ���ģʽ     
    PWMCON0=0x4f;//01011111
    PWMCON1=0x00; 
    OVDCOND=0x00;
    
    PTPERH=periodH;//ȷ��PWM����
    PTPERL=periodL;  //4M 4KHz 0x20 must be 60.68K 
    //SET_DUTY(60);
    /*��PDC������λ���������PTPER����ռ�ձ�Ϊ100%*/
    /*
    PDC0H=dutyH;//ȷ��ռ�ձ�
    PDC0L=dutyL;//80 100%
    PDC1H=dutyH;//ȷ��ռ�ձ�
    PDC1L=dutyL;
    PDC3H=dutyH;//ȷ��ռ�ձ�
    PDC3L=dutyL;
    */
    PTMRL=0;
    PTMRH=0;
    PTEN=1;//PWMʹ��,ʱ�����ϼ���
}


