/*
* UartRosa.h
*
* Created: 11/19/2014 4:11:49 PM
* Author: Eduardo Elael
*/


#ifndef __UARTROSA_H__
#define __UARTROSA_H__

#include "UartStd.h"

namespace Rosa{
	class UartRosa
	{
		//variables
		private:
		const UartStd* uart;

		//functions
		public:
		UartRosa(const UartStd& uart_rosa): uart(&uart_rosa){}
		~UartRosa();
		
		void send_msg(uint8_t data_length, uint8_t *msg);
		uint8_t read_msg(uint8_t& data_length, uint8_t *msg);
		uint8_t read_msg(uint8_t& data_length, uint8_t *msg, float byte_timeout);

	}; //UartRosa
}
#endif //__UARTROSA_H__
