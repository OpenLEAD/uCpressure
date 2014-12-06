/*
* RosaSensors.cpp
*
* Created: 11/18/2014 7:13:11 PM
*  Author: Renan
*/


#include <avr/io.h>
#include "RosaUtils.h"
#include "UartRosa.h"
#include "Velki485.h"
using namespace Rosa;

int main(void)
{
	
	// Crystal Oscillator division factor: 1	
	CLKPR=0x80;
	CLKPR=0x00;
	
	
	ROSAPortsConfig();
	
	OutBit TX_485_en1(PORTC,4), RX_485_en1(PORTC,5), Eth_rst(PORTC,6), Eth_relay(PORTA,0);
	Eth_relay.unset();
	Eth_rst.unset();
	for(uint8_t i =0; i<50;i++)
	_delay_ms(200);
	
	uint8_t msg[3] = {0xAB, 0xCD, 0xEF};
	uint8_t msg2[2] = {0xA5, 0xB6};
	UART1.enable();
	UART0.enable();
	UartRosa pccom = UartRosa(UART1);
	//Rs485Std siow = Rs485Std(UART0,TX_485_en1,RX_485_en1);
	Velki485 velki = Velki485(UART0,TX_485_en1,RX_485_en1);
	uint8_t pressure[9];
	uint8_t readed = 1;
	uint8_t errormsg[2] = {0xEE,0xBB};
	
	uint8_t data[10];
	uint8_t command_size;
	uint8_t response_size;
	const uint8_t timeout = 2;
	const uint8_t max_command_size = 10;
	const uint8_t max_response_size = 10;
	
	while(1)
	{
		command_size = max_command_size;
		response_size = max_response_size;
		
		UART1.read_stream(command_size,data,timeout);
		
		if(velki.forward(data,command_size,data,response_size))
		UART1.send_stream(response_size,data);
		else
		UART1.send_stream(2,errormsg);
		
		
// 		if(velki.read_pressure(pressure))
// 		pccom.send_msg(9,pressure);
// 		else{
// 			pccom.send_msg(2,errormsg);
// 			pccom.send_msg(9,pressure);
// 		}
// 		do
// 		{
// 			pccom.read_msg(readed,msg2);
// 		} while (msg2[0]!=0xAC);
	}
}