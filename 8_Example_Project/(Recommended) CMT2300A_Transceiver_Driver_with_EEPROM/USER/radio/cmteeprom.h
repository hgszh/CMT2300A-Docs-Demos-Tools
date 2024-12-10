/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: (1) "AS IS" WITH NO WARRANTY; AND
 * (2)TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, CMOSTEK SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * Copyright (C) CMOSTEK SZ.
 */

/*!
 * @file    radio.h
 * @brief   Generic radio handlers
 *
 * @version 1.2
 * @date    Jul 17 2017
 * @author  CMOSTEK R@D
 */
 
#ifndef __EEPROM_H
#define __EEPROM_H

#include "typedefs.h"
#include "cmt2300a.h"
#include "time_server.h"


#ifdef __cplusplus 
extern "C" { 
#endif
extern u8 go_eeprom(void);
extern  void Step1(void);
extern  void Step2(u8 addr);
extern  void Step3(u8 addr,u16 datatemp);
extern  u16 Step4(u8 addr);
extern  void Step5(void);


#ifdef __cplusplus 
} 
#endif

#endif
