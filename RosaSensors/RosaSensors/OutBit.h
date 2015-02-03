/*
* OutBit.h
*
* Created: 11/18/2014 7:32:49 PM
*  Author: Eduardo Elael
*/

#include <avr/io.h>

#ifndef OUTBIT_H_
#define OUTBIT_H_

class OutBit{
	
	private:
	volatile uint8_t * pt;
	uint8_t wbit;
	
	public:
	OutBit(volatile uint8_t &port, char bit, bool initial = true ): pt(&port), wbit(bit) {
		if(initial)
		set();
		else
		unset();
		
		*(pt - 1) |= 1<<wbit; //Set Output Bit
	};
	
	
	bool operator==(bool value){return (bool)(*this)==value;}
	
	operator bool() const{return ((*(pt-2) & 1<<wbit)==(1<<wbit));}
	
	void operator=(bool value);
	
	void operator=(int value){operator=(value==(int)1);}
	
	void operator=(OutBit value){operator=((bool)value);}

	inline OutBit toggle(){*pt = *pt ^ 1<<wbit;	return *this;}

	inline OutBit set(){*pt = *pt | 1<<wbit;	return *this;}
	
	inline OutBit unset(){*pt = *pt & ~(1<<wbit);	return *this;}
	
	inline bool get(){return (bool)(*this);}
	
};

#endif /* OUTBIT_H_ */