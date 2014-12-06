/*
* Rs485Std.h
*
* Created: 11/19/2014 5:09:49 PM
* Author: Eduardo Elael
*/


#ifndef __RS485STD_H__
#define __RS485STD_H__

#include "UartStd.h"
#include "OutBit.h"

namespace Rosa
{
	class Rs485Std
	{
		//variables
		private:
		const UartStd* uart;
		OutBit* tx_enable;
		OutBit* rx_enable;
	
	
	
		//functions
		public:
		Rs485Std(const UartStd& uart485, OutBit& tx_enable_485, OutBit& rx_enable_485):
		uart(&uart485),
		tx_enable(&tx_enable_485),
		rx_enable(&rx_enable_485){
			tx_enable->unset();
			rx_enable->set();
			}
		
		void send_msg(uint8_t data_length, uint8_t *msg);
		uint8_t read_msg(uint8_t data_length, uint8_t *msg);
		bool read_msg( uint8_t& data_length, uint8_t *msg, uint8_t byte_timeout, uint8_t response_timeout);
		void flush(void){uart->flush();}
		
		
		
		~Rs485Std();
	
	}; //Rs485Std
}

#endif //__RS485STD_H__
