#include "pic18f4431.h"
#include <stdlib.h>
#include "AD.h"
#include "motor.h"
#include "PWM.h"
#include "LCD.h"
#include <math.h>


void AD_INIT()
{
    TRISA0=1;//AN0
    TRISA1=1;//AN1
        
    ADCON0=0x38;//0011 1000
    ADCON1=0x18;//0001 1000
    ADCON2=0xc0;//11000000
    //ADCON3=0x00;//д╛хо
    //ADCHS=0x00;//д╛хо 
       
    //ADIP=0;
    //ADIE=1;
    ADIF=0;
    ADON=1;
    //delay(1000);
    GODONE=1;
}

void readAD()
{
    while(ADIF==0);
    //TEMP_VALUE=-0.2223*(ADRESH & 0x03)+0.0048*ADRESL+184;
    float tmp=1.25*(ADRESH & 0x03)+0.0048*ADRESL;
    AD_RES=(1.25*(ADRESH & 0x03)+0.0048*ADRESL);
    AD_RES=AD_RES*14.208;
    //AD_RES*=10.0;
    //TEMP_VALUE=-0.17817*(1.25*(ADRESH & 0x03)+0.0048*ADRESL);
    //TEMP_VALUE=(-0.22*(ADRESH & 0x03)-0.001*ADRESL)+184.811;
    //if(tmp<=0.48 || tmp>=1.00)
    {
        //tmp=tmp*434.027;
        //TEMP_VALUE=tmp*1000.0;
    }
    //else
    {
        //TEMP_VALUE=(5.506-sqrt(30.316036+0.00704*(870.6-tmp*1000.0)))/(-0.00352)+30.0;
    }
    TEMP_VALUE=182.25617-176.2632*tmp;
    ADIF=0;
}

