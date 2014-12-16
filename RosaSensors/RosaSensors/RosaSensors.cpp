/*
* RosaSensors.cpp
*
* Created: 11/18/2014 7:13:11 PM
*  Author: Renan
*/

//MICROCONTROLLER CLOCK******************************************************************************************************************
#define F_CPU 8000000UL

//USART PROTOCOL DEFINITIONS****************************************************************************************************
#define SOT                         0x01
#define EOT                         0x02
#define OK                          0x00
#define ERRO_GETMSG_GETTING_EOT	    0xFF

//******************************************************************************************************************************
#define BLUE

#include <avr/io.h>
#include "OutBit.h"
#include "UartRosa.h"
#include "Velki485.h"
#include <string.h>
using namespace Rosa;

int main(void)
{
	
	// Crystal Oscillator division factor: 1
	CLKPR=0x80;
	CLKPR=0x00;
	
	UART1.set_baud(250000); //PC
	UART0.set_baud(9600); //SIOW
	UART1.enable();
	UART0.enable();
	
	#ifdef BLUE
		OutBit EthRelay(PORTA,0), Reset(PORTC,6), InductiveLeftRelay(PORTA,7),
		InductiveRightRelay(PORTA,6), InclinometerRelay(PORTC,0), PressureRelay(PORTC,2), TX_485_en1(PORTC,4);
	
		PressureRelay.unset();
		InclinometerRelay.unset();
		InductiveRightRelay.unset();
		InductiveLeftRelay.unset();
		EthRelay.unset();
		Reset.unset();
	#else
		OutBit TX_485_en1(PORTA,4);
	#endif
	
	OutBit RX_485_en1(PORTC,5);
	Velki485 velki = Velki485(UART0,TX_485_en1,RX_485_en1);
	
	uint8_t pcdata[10];
	uint8_t pressuredata[10];
	uint8_t command_size;
	uint8_t response_size;
	const uint8_t timeout = 10;
	const uint8_t max_command_size = 10;
	const uint8_t max_response_size = 10;
	
	
	while(1)
	{
		command_size = max_command_size;
		response_size = max_response_size;
		
		UART1.read_stream(command_size,pcdata,timeout);
		
		if(velki.forward(pcdata,command_size,pressuredata,response_size))
		UART1.send_stream(response_size,pressuredata);
		
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