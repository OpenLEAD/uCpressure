/*
* UartStd.cpp
*
* Created: 11/18/2014 8:50:11 PM
*  Author: Renan & Eduardo Elael
*/

#include "UartStd.h"

namespace Rosa{
	
	const UartStd UART1(
	&UCSR1A,
	&UCSR1B,
	&UCSR1C,
	&UBRR1,
	&UDR1
	);
	const UartStd UART0(
	&UCSR0A,
	&UCSR0B,
	&UCSR0C,
	&UBRR0,
	&UDR0
	);
	void UartStd::enable(void) const
	{
		/* Set baud rate */
		*(UBRR) = (((F_CPU / (config->baud * 16UL))) - 1);
		/* Set frame format: 8data, no parity & 2 stop bits */
		*(UCSRC) = (1<<UCSZ1) | (1<<UCSZ0) | (config->stopbit << USBS);
		/* Enable receiver and transmitter */
		*(UCSRB) = (1<<RXEN) | (1<<TXEN);// | (1<<RXCIE);
	}
	uint8_t UartStd::read(bool wait, bool* status) const
	{
		uint8_t readed,data;
		
		if (wait){
			while (((readed=*(UCSRA)) & RX_COMPLETE )==0) continue;
		}
		else{
			if (((readed=*(UCSRA)) & RX_COMPLETE)==0){
				if (status != NULL)
				*status = false;
				return 0;
			}
		}
		
		data = *(UDR);
		
		if (status != NULL)
		{
			if ((readed & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
			*status = true;
			else
			*status = false;
		}
		
		return data;
		
	}

	bool UartStd::read( float timeout_ms, uint8_t* data) const
	{
		uint8_t readed;
		TCCR1B = 0; //set NO CLOCK TCCR1A=0, TCCR1B = 0b101;
		TCNT1 =(uint16_t) 0; //Clear counter
		OCR1A = (uint16_t) (timeout_ms/1000.0 * F_CPU/1024.0); //set number of cycles to compare (with PRESCALER 1024 set on 0b101<<CS0)
		
		if(TIFR1 & (1<<OCF1A))
		TIFR1 = 1<<OCF1A; //Clear compare flag
		
		TCCR1B = 0b101; //set PRESCALER 1024
		
		while (((readed=*(UCSRA)) & RX_COMPLETE )==0){
			if(TIFR1 & (1<<OCF1A)){ //Check for compare flag each cycle
				TCCR1B = 0; //set NO CLOCK
				TIFR1 = 1<<OCF1A;
				return false;
			}
		}
	TCCR1B = 0; //set NO CLOCK}

	*data = *UDR;
	
	if (readed & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))
	return false;
	
	return true;
}

void UartStd::read( float timeout_ms, uint8_t& size , uint8_t* data ) const
{
	
}

void UartStd::send(uint8_t data) const{
	*(UCSRA) |= (1<<TXC);
	while ((*(UCSRA) & DATA_REGISTER_EMPTY)==0);
	*(UDR) = data;
}

bool UartStd::transmit_ongoing( void ) const
{
	if ((*(UCSRA) & (1<<TXC)) == 0)
	return true;
	else {
		*(UCSRA) |= (1<<TXC);
		return false;
	}

}

void UartStd::flush( void ) const
{
	uint8_t dummy;
	while (*UCSRA & RX_COMPLETE)
	dummy = *UDR;
}



}