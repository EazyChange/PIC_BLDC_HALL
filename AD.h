/* 
 * File:   AD.h
 * Author: Administrator
 *
 * Created on 2015年12月12日, 下午9:32
 */

#ifndef AD_H
#define	AD_H

//#include "pic18f4431.h"

bit OverTEMP=0;
float AD_RES=0.0;
float TEMP_VALUE=0.0;

void AD_INIT();
void readAD();

#endif	/* AD_H */

