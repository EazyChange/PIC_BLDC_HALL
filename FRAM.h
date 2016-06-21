/* 
 * File:   FRAM.h
 * Author: Administrator
 *
 * Created on 2015年12月2日, 下午7:49
 */

#ifndef FRAM_H
#define	FRAM_H

#define CS_R  RD3

void FRAM_INIT();
void write_F(unsigned char t,unsigned int address);
unsigned char read_F(unsigned int address);
void DataClean(unsigned int b_add,unsigned int e_add);
void Fill_Data(unsigned char num);

#endif	/* FRAM_H */

