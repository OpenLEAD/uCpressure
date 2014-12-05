/*
 * RosaUtils.h
 *
 * Created: 11/18/2014 7:15:47 PM
 *  Author: (Marco Xaud & Renan Freitas) Eduardo Elael
 */ 


#ifndef ROSAUTILS_H_
#define ROSAUTILS_H_

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

//SOME DEFINITIONS*************************************************************************************************************
#define OVR 3
#define FRAMING_ERROR (1<<FE)
#define PARITY_ERROR (1<<UPE)
#define DATA_OVERRUN (1<<OVR)
#define DATA_REGISTER_EMPTY (1<<UDRE)
#define RX_COMPLETE (1<<RXC)
#define DATA_REGISTER_EMPTY1 (1<<UDRE1)
#define DATA_REGISTER_EMPTY0 (1<<UDRE0)
#define RX_COMPLETE0 (1<<RXC0)
#define RX_COMPLETE1 (1<<RXC1)

//SSEUP Parameters***************************************************************************************************************
#define COMANDO        0
#define POSITION_NR 0
#define POSITION_ONOFF 0
#define MASK_BYTE1 0
#define MASK_BYTE2 0

//USART PROTOCOL DEFINITIONS****************************************************************************************************
#define SOT                         0x01
#define EOT                         0x02
#define OK                          0x00
#define ERRO_GETMSG_GETTING_EOT	    0xFF
#define USART_BAUDRATE 9600 //UART Baudrate //ALTERADO DE 9600
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1) //Calc for UART baudrate set

//SSEUP Commands****************************************************************************************************************
#define HEART_BEAT	0xBC
#define VELKI_READ	0xA0
#define DEVICERELAY_CONFIG_SINGLE	0xA1
#define DEVICERELAY_CONFIG_MASK	0xA2
#define DEVICERELAY_ONOFF_SINGLE	0xA3
#define DEVICERELAY_ONOFF_MASK	0xA4
#define DEVICERELAY_STATUSREAD	0xA5
#define REPLY_MODULE_DATA_VOLTAGES	0xB0
#define REPLY_MODULE_DATA_CURRENTS	0xB1
#define REPLY_MODULE_DATA_TEMP_HUM	0xB2
#define DEVICERELAY_STATUSREPLY	0xD2

//EXTERNAL ADC ***********************************************************************************************************************
#define CREG_WRITE	0x80
#define CREG_SEQ	0x40
#define CREG_ADD3	    0x20
#define CREG_ADD2	0x10
#define CREG_ADD1	0x08
#define CREG_ADD0	0x04
#define CREG_PM1	0x02
#define CREG_PM0	0x01
#define CREG_SHADOW	0x80
#define CREG_WEAK	0x40
#define CREG_RANGE	0x20
#define CREG_CODING	0x10

#define ADC_VREF 3264 //mV
#define ADC_BIAS 2369 //mV
#define ADC_SENS 185 //mv/A

// INTERNAL ADC
#define R1 10000
#define R2 1300
#define VIN_MAX 33600


//adr  command  r/w
#define STATUS_REG_W 0x06   //000   0011    0
#define STATUS_REG_R 0x07   //000   0011    1
#define MEASURE_TEMP 0x03   //000   0001    1
#define MEASURE_HUMI 0x05   //000   0010    1
#define SHT_RESET        0x1e   //000   1111    0


// RS485 está na UART0
// Sch-remote está na UART1
	


// ADC Functions 
void AD7490Measurements(unsigned char *currents);
void AD7490Calc(unsigned char *currents);

//Voltage measures
void measureVoltageMUX(unsigned char muxNum , unsigned char *voltages);
void VoltageCalc(unsigned char *voltages);


// Debug Functions
unsigned send_msg(unsigned msg_length, unsigned char *msg);
void checkpoint(unsigned char rastreamento);
void checkpoint(unsigned short rastreamento);
void checkpoint(float rastreamento);

//SPI Functions
void SPI_MasterInit(void);
unsigned char SPI_MasterTransmit(unsigned char reg_address);

// functions for commands definition
void heartBeat();
void heartBeat2();
void singleToRelaysMask();
void singleToConfigMask();
void setAllRelaysROSA();

/* UART Functions*/

// RS485 Enable
void enable_TX_485_1 (void);
void enable_RX_485_1 (void);
// RS485 com SOT e EOT
unsigned send_485(unsigned data_length, unsigned char *data);
unsigned get_485(unsigned char init, unsigned char *data);
/*******************************/

// Functions for Pressure Sensor
void InitVelki(void);
void ReadVelki(void);

// Ports configuration
void ROSAPortsConfig();


/* UART Functions*/
char getchar1(void); // Get a character from the USART1 Receiver
char getchar0(void); // Get a character from the USART0 Receiver
void putchar1(char c); // Write a character to the USART1 Transmitter
void putchar0(char c); // Write a character to the USART0 Transmitter
// SEND UART msg
unsigned send_msg_uart1(unsigned data_length, unsigned char *data);
unsigned send_msg_uart0(unsigned data_length, unsigned char *data);
// GET UART msg
unsigned get_msg1(unsigned char *data);
unsigned get_msg0(unsigned char *data);
// USART
void USART1_Init(void);
void USART0_Init(void);

//extern OutBit TX_485_en1, RX_485_en1;


#endif /* ROSAUTILS_H_ */