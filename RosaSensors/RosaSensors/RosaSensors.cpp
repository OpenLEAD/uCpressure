/*
* RosaSensors.cpp
*
* Created: 11/18/2014 7:13:11 PM
*  Author: Renan
*/

//******************************************************************************************************************************

#include <avr/io.h>
#include "OutBit.h"
#include "UartRosa.h"
#include "Velki485.h"
#include <string.h>
using namespace Rosa;

#define INDUTIVOS

int main(void)
{
	
	// Crystal Oscillator division factor: 1
	CLKPR=0x80;
	CLKPR=0x00;
	
	UART1.set_baud(250000); //PC
	UART0.set_baud(9600); //SIOW
	UART1.enable();
	UART0.enable();
	
	#ifdef INDUTIVOS
	OutBit RX_485_Pressure(PORTA,7,true), TX_485_Pressure(PORTA,4,false);
	#endif
	
	#ifdef SONAR
	OutBit RX_485_Pressure(PORTC,0,true), TX_485_Pressure(PORTC,1,false),
			RX_485_Seaking(PORTC,2,true), TX_485_Seaking(PORTC,3,false),
			RX_485_PTU(PORTC,4,true), TX_485_PTU(PORTC,5,false),
			RX_485_Base(PORTC,6,true), TX_485_Base(PORTC,7,false);
	#endif
	
	
	Velki485 velki = Velki485(UART0,TX_485_Pressure,RX_485_Pressure);
	
	uint8_t pcdata[10];
	uint8_t pressuredata[10];
	uint8_t command_size;
	uint8_t response_size;
	const uint8_t timeout = 5;
	const uint8_t max_command_size = 10;
	const uint8_t max_response_size = 10;
	
	
	while(1)
	{
		command_size = max_command_size;
		response_size = max_response_size;
		
		UART1.read_stream(command_size,pcdata,timeout);
		
		if(velki.forward(pcdata,command_size,pressuredata,response_size))
		UART1.send_stream(response_size,pressuredata);
 		else{
 			uint8_t timeoutanswer[9]={0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb};
 			timeoutanswer[0]=0xaa;
 			timeoutanswer[1]=command_size;
 			memcpy(timeoutanswer+2,pcdata,command_size>7? 7 : command_size);
 			UART1.send_stream(9,timeoutanswer);
 		}
		
	}
}


// 	//TESTE ECHO PC
// 	while(1){
// 		command_size = max_command_size;
// 		UART1.read_stream(command_size,pcdata,timeout);
// 		UART1.send_stream(command_size,pcdata);
// 	}

// 	//TESTE PING SIOW read_pressure
// 	while(1)
// 	velki.read_pressure(pressure);

// 	//TESTE PING SIOW forward
// 	while(1){
// 		uint8_t testdata[5] = {0xfa, 0x49, 0x01, 0xa1, 0xa7};
// 		uint8_t testsize = 5;
// 		velki.forward(testdata,testsize,testdata,testsize);
// 	}

// 	//TESTE PING SIOW forward
// 	uint8_t testdata[5] = {0xfa, 0x49, 0x01, 0xa1, 0xa7};
// 	while(1)
// 	{
// 		command_size = max_command_size;
// 		response_size = max_response_size;
//
// 		memcpy(pcdata,testdata,5);
// 		command_size = 5;
//
// 		UART1.read_stream(command_size,pcdata,timeout);
//
// 		if(velki.forward(pcdata,command_size,pressuredata,response_size))
// 		continue;
// 	}

// 	//TESTE pc->uc->siow
// 	while(1)
// 	{
// 		command_size = max_command_size;
// 		response_size = max_response_size;
//
// 		UART1.read_stream(command_size,pcdata,timeout);
//
// 		if(velki.forward(pcdata,command_size,pressuredata,response_size))
// 		continue;
// 	}

// 	//TESTE pc<->uc->siow w/ECHO
// 	while(1)
// 	{
// 		command_size = max_command_size;
// 		response_size = max_response_size;
//
// 		UART1.read_stream(command_size,pcdata,timeout);
// 		UART1.send_stream(command_size,pcdata);
// 		if(velki.forward(pcdata,command_size,pressuredata,response_size))
// 		continue;
// 	}