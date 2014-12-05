/*
* UartRosa.cpp
*
* Created: 11/19/2014 4:11:49 PM
* Author: Eduardo Elael
*/


#include "UartRosa.h"

namespace Rosa
{
	// default destructor
	UartRosa::~UartRosa()
	{
	} //~UartRosa

	void UartRosa::send_msg( uint8_t data_length, uint8_t *msg )
	{
		uart->send(SOT);
		uart->send(data_length);
		for (uint8_t i=0; i < data_length; i++)
		uart->send(msg[i]);
		uart->send(EOT);
	}

	uint8_t UartRosa::read_msg( uint8_t& data_length, uint8_t *msg)
	{
		
		// Aguarda ate a chegada do byte indicando o inicio de transmissao do PC ao AVR Master
		
		while(uart->read()!=SOT);
		// Aguarda o tamanho da mensagem
		data_length = uart->read();
		for (uint8_t i = 0 ; i < data_length ; i++)
		msg[i] = uart->read();
		
		if (uart->read()!=EOT) return (ERRO_GETMSG_GETTING_EOT);
		
		return (OK);
		
	}

	uint8_t UartRosa::read_msg( uint8_t& data_length, uint8_t *msg, float byte_timeout )
	{
		
		// Aguarda ate a chegada do byte indicando o inicio de transmissao do PC ao AVR Master
		
		while(uart->read()!=SOT);
		// Aguarda o tamanho da mensagem
		data_length = uart->read();
		for (uint8_t i = 0 ; i < data_length ; i++)
		msg[i] = uart->read();
		
		if (uart->read()!=EOT) return (ERRO_GETMSG_GETTING_EOT);
		
		return (OK);
		
	}

}
