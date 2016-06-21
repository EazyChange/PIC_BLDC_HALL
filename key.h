/* 
 * File:   key.h
 * Author: Administrator
 *
 * Created on 2016年3月10日, 下午3:26
 */

#ifndef KEY_H
#define	KEY_H

#define S1 RD6  //+
#define S2 RD7  //-
#define S3 RB6  //SET
#define S4 RB7  //OK
/*
#define PLUS   1
#define MINUS  2
#define SET    3
#define OK     4
#define PLUS1  5
#define MINUS1 2
#define NEXT   7
#define OK1    8
*/
void KEY_INIT();
unsigned char KeyScan();

#endif	/* KEY_H */

