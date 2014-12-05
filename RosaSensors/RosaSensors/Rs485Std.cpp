/*
* Rs485Std.cpp
*
* Created: 11/19/2014 5:09:49 PM
* Author: Eduardo Elael
*/


#include "Rs485Std.h"

namespace Rosa
{
	// default destructor
	Rs485Std::~Rs485Std()
	{
	} //~Rs485Std

	void Rs485Std::send_msg( uint8_t data_length, uint8_t *msg )
	{
		tx_enable->set();
		for (uint8_t i = 0 ; i < data_length ; i++)
		uart->send(msg[i]);
		
		while(uart->transmit_ongoing());
		tx_enable->unset();
		
	}

	bool Rs485Std::read_msg( uint8_t& data_length, uint8_t *msg, float byte_timeout, float response_timeout)
	{
		rx_enable->unset();
		
		if(!uart->read(response_timeout,msg)){
			data_length=0;
			return false;
		}
		
		uint8_t received_length = 1;
		
		for (uint8_t i = 1 ; i < data_length ; i++, received_length++)
		if(!uart->read(byte_timeout,msg+i)){
			data_length = received_length;
			return false;
		}
		
		rx_enable->set();
		
		return true;
	}

	uint8_t Rs485Std::read_msg( uint8_t data_length, uint8_t *msg )
	{
		rx_enable->unset();
		
		for (uint8_t i = 0 ; i < data_length ; i++)
		msg[i] = uart->read();
		
		rx_enable->set();
		
		
		return (OK);
		
	}

}
