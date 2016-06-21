#include <stdio.h>
#include <stdlib.h>
#include <pic18f4431.h>
#include "key.h"
#include "Delays.h"

void KEY_INIT()
{
    TRISD6=1;TRISD7=1;TRISB6=1;TRISB7=1;TRISC1=1;TRISC2=1;//按键初始化
}
//按键1-4短按对应1-4，长按对应5-8
unsigned char KeyScan()
{
    long keytime=0;
    if(S1==0 || S2==0 || S3==0 || S4==0)//有键按下
    { 
        delay(500);
        if(S1==0)//是S1
        {
            while(S1==0)
            {    
                if(keytime>=60000)
                {}
                else
                {keytime++;}
            }//等待键释放
            if(keytime>=59999)
            {return 5;}
            else
            {return 1;}
        }
        if(S2==0)
        {
            while(S2==0)
            {    
                if(keytime>=60000)
                {}
                else
                {keytime++;}
            }
            if(keytime>=59999)
            {return 6;}
            else
            {return 2;}
        }
        if(S3==0)
        {
            while(S3==0)
            {    
                if(keytime>=60000)
                {}
                else
                {keytime++;}
            }
            if(keytime>=59999)
            {return 7;}
            else
            {return 3;}  
        }
        if(S4==0)
        {
            while(S4==0)
            {    
                if(keytime>=60000)
                {}
                else
                {keytime++;}
            }
            if(keytime>=59999)
            {return 8;}
            else
            {return 4;}
        }
    }
    return 0;
}
