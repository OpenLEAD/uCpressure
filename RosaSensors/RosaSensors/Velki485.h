/*
* Velki485.h
*
* Created: 11/24/2014 10:38:05 AM
* Author: Eduardo Elael
*/


#ifndef __VELKI485_H__
#define __VELKI485_H__


#include "Rs485Std.h"

namespace Rosa{

	class Velki485: private Rs485Std
	{
		private:
		bool initialized; //TO DO
		
		//functions
		public:
		Velki485(const UartStd& uart485, OutBit& tx_enable_485, OutBit& rx_enable_485):
		Rs485Std(uart485,tx_enable_485,rx_enable_485), initialized(false)
		{
			initialize();
		}
		
		bool read_pressure(uint8_t pressure_msg[9]);
		bool forward( uint8_t* command, uint8_t command_size, uint8_t* response, uint8_t& response_size );
		bool isinitialized(void){return initialized;}
		bool initialize(void);
		
		
		~Velki485();

	}; //Velki485

}

#endif //__VELKI485_H__
