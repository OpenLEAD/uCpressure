/*
* UartStd.h
*
* Created: 11/18/2014 8:18:25 PM
*  Author: Renan & Eduardo Elael
*/


#ifndef UARTSTD_H_
#define UARTSTD_H_
#include "RosaDefines.h"

//SOME DEFINITIONS*************************************************************************************************************
#define OVR 3
#define FRAMING_ERROR (1<<FE)
#define PARITY_ERROR (1<<UPE)
#define DATA_OVERRUN (1<<OVR)
#define DATA_REGISTER_EMPTY (1<<UDRE)
#define RX_COMPLETE (1<<RXC)
#define DATA_REGISTER_EMPTY1 (1<<UDRE1)
#define DATA_REGISTER_EMPTY0 (1<<UDRE0)
#define RX_COMPLETE0 (1<<RXC0)
#define RX_COMPLETE1 (1<<RXC1)

/*******************************/
namespace Rosa{
	struct UartConfig{
		uint32_t baud;
		bool parity; //não implementado
		uint8_t stopbit; //não implementado
		void (*interrupt)(void);
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
		
		void set_baud(uint32_t uart_baud) const{config->baud=uart_baud;} //não muda o baud de um porta já ativa
		uint8_t get_baud(void) const{return config->baud;}
		
		void enable(void) const;
		
		uint8_t read(bool* status = NULL) const;
		bool read(uint8_t timeout_ms, uint8_t* data ) const;
		void read_stream(uint8_t& size , uint8_t* data, uint8_t timeout_byte) const;
		void flush(void) const;
		void send(uint8_t data) const;
		void send_stream(uint8_t size , uint8_t* data) const{ for(uint8_t i=0; i < size; i++) send(data[i]);}
		bool transmit_ongoing(void) const;
		
		void link(void (*f)(void)) const{config->interrupt=f;}
		
	};
	
	extern const UartStd UART1;
	extern const UartStd UART0;
	
	
	
}


#endif /* UARTSTD_H_ */