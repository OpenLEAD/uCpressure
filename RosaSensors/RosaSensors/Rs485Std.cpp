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
		
		uart->send_stream(data_length,msg);
		
		while(uart->transmit_ongoing());
		tx_enable->unset();
		
	}

	bool Rs485Std::read_msg( uint8_t& data_length, uint8_t *msg, uint8_t byte_timeout, uint8_t response_timeout)
	{
		rx_enable->unset();
		
		if(!uart->read(response_timeout,msg)){
			data_length=0;
			rx_enable->set();
			return false;
		}
		
		uint8_t max_length = data_length;
		
		for ( data_length = 1 ; data_length < max_length ; data_length++)
		if(!uart->read(byte_timeout,msg+data_length)){
			rx_enable->set();
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
