#ifndef DS18B20_H
#define DS18B20_H

#include <pic18f4431.h>



//void delay(unsigned char x);
void WriteByte(unsigned char data);
unsigned char ReadByte(void);
unsigned char INIT_DS18B20();
//unsigned char INIT_DS18B20_1();
unsigned char GetTemp();



#endif