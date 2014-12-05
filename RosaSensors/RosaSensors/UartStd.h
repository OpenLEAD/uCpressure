/*
* UartStd.h
*
* Created: 11/18/2014 8:18:25 PM
*  Author: Renan & Eduardo Elael
*/


#ifndef UARTSTD_H_
#define UARTSTD_H_
#include "RosaUtils.h"

/*******************************/
namespace Rosa{
	struct UartConfig{
		uint16_t baud;
		bool parity; //não implementado
		uint8_t stopbit; //não implementado
		void (*interrupt)(void);
		UartConfig():baud((uint16_t)9600),parity(false),stopbit(0),interrupt(NULL){}
	};
	
	class UartStd {
		private:
		volatile uint8_t* UCSRA;
		volatile uint8_t* UCSRB;
		volatile uint8_t* UCSRC;
		volatile uint16_t* UBRR;
		volatile uint8_t* UDR;
		
		UartConfig* config;
		
		public:
		UartStd(volatile uint8_t* uart_UCSRA,
		volatile uint8_t* uart_UCSRB,
		volatile uint8_t* uart_UCSRC,
		volatile uint16_t* uart_UBRR,
		volatile uint8_t* uart_UDR):
		UCSRA(uart_UCSRA),
		UCSRB(uart_UCSRB),
		UCSRC(uart_UCSRC),
		UBRR(uart_UBRR),
		UDR(uart_UDR){
			config = (UartConfig*) malloc(sizeof(UartConfig));
			config->baud = 9600;
			config->parity = false;
			config->stopbit = 0;
			config->interrupt = NULL;
		}
		
		void set_baud(uint8_t uart_baud) const{config->baud=uart_baud;} //não muda o baud de um porta já ativa
		uint8_t get_baud(void) const{return config->baud;}
		
		void enable(void) const;
		
		uint8_t read(bool* status = NULL) const;
		bool read(float timeout_ms, uint8_t* data ) const;
		void read_stream(uint8_t& size , uint8_t* data, float timeout_byte) const;
		void flush(void) const;
		void send(uint8_t data) const;
		void send_stream(uint8_t size , uint8_t* data);
		bool transmit_ongoing(void) const;
		
		void link(void (*f)(void)) const{config->interrupt=f;}
		
	};
	
	extern const UartStd UART1;
	extern const UartStd UART0;
	
	
	
}


#endif /* UARTSTD_H_ */