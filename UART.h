/* 
 * File:   UART.h
 * Author: Administrator
 *
 * Created on 2015��11��3��, ����10:30
 */

#ifndef UART_H
#define	UART_H

void UART_INIT();
void Send_Hex(unsigned char data);
void Send_Str(unsigned char *data);
unsigned char Rev_Hex();

#endif


