/*
* Velki485.cpp
*
* Created: 11/24/2014 10:38:05 AM
* Author: Eduardo Elael
*/


#include "Velki485.h"
#include <util/crc16.h>
#include <util/delay.h>
#include <string.h>

namespace Rosa {

	// default destructor
	Velki485::~Velki485()
	{
	} //~Velki485

	bool Velki485::read_pressure( uint8_t pressure_msg[9])
	{
		
		uint8_t ReqPressure1[5] = {0xfa, 0x49, 0x01, 0xa1, 0xa7}; //0xFA = 250 - Broadcast; 0x49 = 73 Read value; 0x01 - Pressure from pressure sensor 1 (bar); 0xa1a7 - CRC
		
		flush();
		send_msg(5,ReqPressure1);
		
		uint8_t received_size = 9;
		if(!read_msg(received_size,pressure_msg,1.8,200))
		if (received_size<5){
			_delay_ms(1);
			uint8_t badanswer[9]={0x12, 0xaa, 0xbb, received_size, 0x12, 0xaa, 0xbb, 0x00, 0x1B};
			memcpy(pressure_msg,badanswer,9);
			return false;
		}
		
		_delay_ms(1);
		
		// CRC16 recebido do sensor
		uint16_t CRCRec = (pressure_msg[received_size-2]<<8) + pressure_msg[received_size-1];

		// Calculo de CRC16 dos dados recebidos
		uint16_t CRC = 0xFFFF;
		for(uint8_t i=0; i < (received_size-2); i++)
		CRC = _crc16_update(CRC, pressure_msg[i]);

		if(CRC != CRCRec){
			uint8_t badcrc[9]={0x00, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x9B, 0x45};
			memcpy(pressure_msg,badcrc,9);			
			return false; //CRC ERROR
		}
		
		if (pressure_msg[1]&(1<<7)){ //EXCEPTION
			if ((pressure_msg[1]&~(1<<7))==0x20) //NOT INITIALIZED
			initialize();
			uint8_t noinit[9]={1,2,3,4,5,6,7,0x71,0x1D};
			memcpy(pressure_msg,noinit,9);
			return false;
		}
		
		
		return true; // NOT ALL FINE - CHECK STAT pressure_msg[6] BYTE!!
		
	}

	bool Velki485::initialize( void )
	{
		
		uint8_t SensorInit[4] = {0xfa, 0x30, 0x04, 0x43};// 0xFA = 250 - Broadcast; 0x30 = 48 initialization; 0x0443 - CRC
		send_msg(4,SensorInit);
		uint8_t answer[10];
		uint8_t received_size = 10;
		
		if(!read_msg(received_size,answer,1.8,100)){
			_delay_ms(1);
			return (initialized = false);
		}
		
		_delay_ms(1);
		
		// CRC16 recebido do sensor
		uint16_t CRCRec = (answer[received_size-2]<<8) + answer[received_size-1];

		// Calculo de CRC16 dos dados recebidos
		uint16_t CRC = 0xFFFF;
		for(uint8_t i=0; i < (received_size-2); i++)
		CRC = _crc16_update(CRC, answer[i]);

		if(CRC != CRCRec || answer[1]!=0x30)
		return (initialized = false); //CRC ERROR
		
		return (initialized = true);
	}

bool Velki485::forward( uint8_t* command, uint8_t command_size, uint8_t* response, uint8_t& response_size )
{
	flush();
	//send_msg(command_size,command);
	
	read_msg(response_size,response,2,200);
	
	_delay_ms(1);
	
	if(response_size==0)
		return false;
		
	return true; // NOT ALL FINE - CHECK STAT pressure_msg[6] BYTE!!
	
}

}
