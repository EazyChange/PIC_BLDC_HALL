/* 
 * File:   DRV0803.h
 * Author: Administrator
 *
 * Created on 2015年12月12日, 下午3:28
 */

#ifndef DRV0803_H
#define	DRV0803_H

#include <pic18f4431.h>

#define EN_GATE RE1
#define CLK_DRV RC5
#define DI_DRV  RC7
#define DO_DRV  RC4
#define CS_DRV  RA5

void DRV_INIT();

#endif	/* DRV0803_H */

