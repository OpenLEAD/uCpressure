/*
 * RosaDefines.h
 *
 * Created: 11/18/2014 7:15:47 PM
 *  Author: (Marco Xaud & Renan Freitas) Eduardo Elael
 */ 


#ifndef ROSADEFINES_H_
#define ROSADEFINES_H_

 //MICROCONTROLLER CLOCK******************************************************************************************************************
#define F_CPU 8000000UL

//LIBRARIES*******************************************************************************************************************************
#include "OutBit.h"
#include <avr/io.h>
#include <util/delay.h> //for delays
#include <avr/interrupt.h> //for interruptions
#include <avr/wdt.h> //for timers
#include <stdio.h> //for T/H sensor
#include <stdlib.h> //for T/H sensor
#include <math.h> //for T/H sensor
#include <stdio.h>
#include <util/crc16.h>
//******************************



//USART PROTOCOL DEFINITIONS****************************************************************************************************
#define SOT                         0x01
#define EOT                         0x02
#define OK                          0x00
#define ERRO_GETMSG_GETTING_EOT	    0xFF



#endif /* ROSADEFINES_H_ */