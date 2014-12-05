/*
 * OutBit.cpp
 *
 * Created: 11/18/2014 7:34:04 PM
 *  Author: Eduardo Elael
 */ 

#include "OutBit.h"

void OutBit::operator=(bool value){
	if(value)
	*pt |= 1<<wbit;
	else
	*pt &= ~(1<<wbit);
}

