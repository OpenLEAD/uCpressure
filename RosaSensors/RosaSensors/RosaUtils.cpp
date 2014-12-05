/*
* RosaUtils.cpp
*
* Created: 11/18/2014 7:42:56 PM
*  Author: Eduardo Elael
*/

/*
* DORIS SC.cpp
*
* Created: 29/01/2014 14:48:00
*  Author: LEAD - DORIS - G2 (Marco Xaud & Renan Freitas)
*/


#include "RosaUtils.h"

// *********************

unsigned char buffer_dados[256];
unsigned char buffer_dados_485[9];
unsigned char numero_bytes;
unsigned char configMask1;
unsigned char configMask2;
unsigned char relaysMask1;
unsigned char relaysMask2;
volatile unsigned data_index; //indicates data index while receiving SSEUP message
volatile unsigned char data_length; //indicates SSEUP message length
volatile unsigned char busy; //flag to treat SSEUP receiving interrupts
volatile unsigned char busy2; //flag to treat SSEUP receiving interrupts

// ROSA Relays
//************************************************************************
//************************************************************************
OutBit Relay02(PORTA,1), Relay03(PORTA,2), Relay04(PORTA,3), Relay05(PORTA,4), Relay06(PORTA,5), Relay07(PORTA,6),
Relay08(PORTA,7), Relay09(PORTC,0), Relay10(PORTC,1), Relay11(PORTC,2), Relay12(PORTC,3), // SOLID-STATE RELAY PORTS
AD_DOUT(PORTB,3), AD_DIN(PORTB,2), AD_SCLK(PORTB,1), AD_NOTCS(PORTB,0); // SPI PORTS
//TX_485_en1(PORTC,4), RX_485_en1(PORTC,5); // 485 ENABLE

//************************************************************************
//************************************************************************

// ADC Functions
//************************************************************************
//************************************************************************
void AD7490Measurements(unsigned char *currents)
{
	unsigned char send1;
	unsigned char send2;
	unsigned char get1;
	unsigned char get2;
	unsigned char ch;

	for (ch = 0 ; ch <= 16 ; ch+=1)
	{
		// building message to be sent
		send1 = 0x00; send2 = 0x00;
		send1 |= CREG_WRITE;
		if (ch<16) {send1 |= (ch<<2);}
		else {send1 |= (0<<2);} // sending dont care
		send1 |= CREG_PM1;
		send1 |= CREG_PM0;
		send2 |= CREG_RANGE;
		send2 |= CREG_CODING;

		AD_NOTCS=0;
		// first byte transmission
		get1 = SPI_MasterTransmit(send1);
		_delay_us(100);
		// second byte transmission
		get2 = SPI_MasterTransmit(send2);
		AD_NOTCS=1;
		_delay_us(100);
		// Getting message
		if (ch>1)
		{
			currents[(ch-2)*2]=get1;
			currents[(ch-2)*2+1]=get2;
		}
	}
}
void AD7490Calc(unsigned char *currents){
	unsigned short tempVar1;
	float finalValue;
	short int tempVar2;
	for (int k = 0;k<30;k+=2)
	{
		if (((unsigned char)((k/2)+1))==((currents[k]&0xF0)>>4)) //verifying if channels ID are consistent
		{
			tempVar1=(((unsigned short)(currents[k]&0x0F))<<8)|((unsigned short)currents[k+1]);
			finalValue=(float)(tempVar1);
			finalValue=1000*((finalValue*ADC_VREF*1.0/4096)-ADC_BIAS)/ADC_SENS;
			tempVar2=(short int)(finalValue);
			currents[k]=(unsigned char)((tempVar2&0xFF00)>>8);
			currents[k+1]=(unsigned char)(tempVar2&0x00FF);
		}
		else
		{
			currents[k] = 0x7F;
			currents[k+1] = 0xFF; //Bad current
		}
	}
}
//************************************************************************
//************************************************************************

//Voltage measures
//************************************************************************
//************************************************************************
void measureVoltageMUX(unsigned char muxNum , unsigned char *voltages)
{
	// ****************** Voltage data ********************
	
	ADCSRA |= (1 << ADEN);
	DIDR0 = 0xFF;
	ADCSRA &= ~((1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0));
	
	ADMUX = muxNum;
	_delay_ms(50);
	ADCSRA |= (1 << ADSC); //start a conversion
	while((ADCSRA & 0x10) == 0){} //wait for conversion to finish
	// Data manipulation
	voltages[muxNum*2+1] = ADCL; //ADCL must be first
	voltages[muxNum*2] = ADCH;
	ADCSRA |= (1 << ADIF);
}
void VoltageCalc(unsigned char *voltages){
	unsigned short int temp;
	float tempf;
	for(int i=0;i<8;i+=2){
		temp = (voltages[i]<<8)|(voltages[i+1]);
		tempf=(float)temp;
		tempf = tempf*1.0*ADC_VREF/(1023);
		temp = (unsigned short int)tempf;
		voltages[i] = (unsigned char)((temp & 0xFF00)>>8);
		voltages[i+1] = (unsigned char)(temp & 0x00FF);
	}

}
//************************************************************************
//************************************************************************


// Debug Functions
//************************************************************************
//************************************************************************
void checkpoint(unsigned char rastreamento)
{
	unsigned char sending485[1];
	sending485[0]=rastreamento;
	send_msg(1,sending485);
}
void checkpoint(unsigned short rastreamento)
{
	unsigned char sendingData[2];
	sendingData[0]=((rastreamento&0xFF00)>>8);
	sendingData[1]=rastreamento&0x00FF;
	send_msg(2,sendingData);
}
void checkpoint(float rastreamento)
{
	unsigned char *p;
	p = (unsigned char *)&rastreamento;
	for(int i=0; i<4; i++){
	send_msg(1,p++);}
}
//************************************************************************
//************************************************************************

//SPI Functions
//************************************************************************
//************************************************************************
void SPI_MasterInit(void)
{
	/* Set MOSI and SCK output, all others input */
	DDRB |= ((1<<DDB2)|(1<<DDB1));
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}
unsigned char SPI_MasterTransmit(unsigned char reg_address)
{
	/* Start transmission */
	SPDR = reg_address;
	//AD_NOTCS=0;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	//AD_NOTCS=1;
	/*Receive the data from MISO line*/
	unsigned char resultadoSPI = SPDR;
	return resultadoSPI;
}
//************************************************************************
//************************************************************************

// functions for commands definition
//************************************************************************
//************************************************************************
void heartBeat()
{
	unsigned char sendingData = (unsigned char)(HEART_BEAT);
	send_msg_uart1(1, &sendingData);
}
void heartBeat2()
{
	unsigned char sendingData[21] = {97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117};
	//unsigned char sendingData[] = "testando";
	send_msg_uart1(21, sendingData);
}
void singleToRelaysMask()
{
	if (POSITION_NR<16 && POSITION_NR>=2 && POSITION_ONOFF<2 && POSITION_ONOFF>=0)  // validate message (must be between 0x02 and 0x0F)
	{
		int pos = (int)(POSITION_NR)%8;
		if (POSITION_NR<8)
		{
			switch (POSITION_ONOFF)
			{
				case 0x00:
				relaysMask2 &= ~(1<<pos);
				break;
				case 0x01:
				relaysMask2 |= (1<<pos);
				break;
				default:
				break;
			}
		}
		else if (POSITION_NR>=8)
		{
			switch (POSITION_ONOFF)
			{
				case 0x00:
				relaysMask1 &= ~(1<<pos);
				break;
				case 0x01:
				relaysMask1 |= (1<<pos);
				break;
				default:
				break;
			}
		}
	}
	else{return;}
}
void singleToConfigMask()
{
	if (POSITION_NR<16 && POSITION_NR>=2 && POSITION_ONOFF<2 && POSITION_ONOFF>=0)  // validate message (must be between 0x02 and 0x0F)
	{
		int pos = (int)(POSITION_NR)%8;
		if (POSITION_NR<8)
		{
			switch (POSITION_ONOFF)
			{
				case 0x00:
				configMask2 &= ~(1<<pos);
				break;
				case 0x01:
				configMask2 |= (1<<pos);
				break;
				default:
				break;
			}
		}
		else if (POSITION_NR>=8)
		{
			switch (POSITION_ONOFF)
			{
				case 0x00:
				configMask1 &= ~(1<<pos);
				break;
				case 0x01:
				configMask1 |= (1<<pos);
				break;
				default:
				break;
			}
		}
	}
	else{return;}
}
void setAllRelaysROSA()
{
	relaysMask1 = relaysMask1&configMask1;
	relaysMask2 = relaysMask2&configMask2;
	
	//Relay01=((relaysMask2 & (1<<1))==0x00);
	//Relay01.unset();
	Relay02=((relaysMask2 & (1<<2))==0x00);
	Relay03=((relaysMask2 & (1<<3))==0x00);
	Relay04=((relaysMask2 & (1<<4))==0x00);
	Relay05=((relaysMask2 & (1<<5))==0x00);
	Relay06=((relaysMask2 & (1<<6))==0x00);
	Relay07=((relaysMask2 & (1<<7))==0x00);
	Relay08=((relaysMask1 & (1<<0))==0x00);
	Relay09=((relaysMask1 & (1<<1))==0x00);
	Relay10=((relaysMask1 & (1<<2))==0x00);
	Relay11=((relaysMask1 & (1<<3))==0x00);
	Relay12=((relaysMask1 & (1<<4))==0x00);
}
//************************************************************************
//************************************************************************

// UART Functions
//******************************************************************************
//******************************************************************************
// Get a character from the USART1 Receiver
char getchar1(void)
{
	char status,data;
	while (1)
	{
		while (((status=UCSR1A) & RX_COMPLETE)==0);
		data=UDR1;
		if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
		return data;
	};
}
// Get a character from the USART0 Receiver
char getchar0(void)
{
	char status,data;
	while (1)
	{
		while (((status=UCSR0A) & RX_COMPLETE0)==0);
		data=UDR0;
		if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
		return data;
	};
}
// Write a character to the USART1 Transmitter
void putchar1(char c)
{
	while ((UCSR1A & DATA_REGISTER_EMPTY)==0);
	UDR1=c;
}
// Write a character to the USART0 Transmitter
void putchar0(char c)
{
	while ((UCSR0A & DATA_REGISTER_EMPTY0)==0);
	UDR0=c;
}
// SEND UART msg
unsigned send_msg_uart1(unsigned data_length, unsigned char *data)
{
	unsigned data_index;
	putchar1(SOT);
	putchar1(data_length);
	for (data_index = 0 ; data_index < data_length ; data_index++)
	putchar1(data[data_index]);
	putchar1(EOT);
	return (OK);
}
unsigned send_msg_uart0(unsigned data_length, unsigned char *data)
{
	unsigned data_index;
	putchar0(SOT);
	putchar0(data_length);
	for (data_index = 0 ; data_index < data_length ; data_index++)
	putchar0(data[data_index]);
	putchar0(EOT);
	return (OK);
}
// GET UART msg
unsigned get_msg1(unsigned char *data)
{
	unsigned data_index;
	unsigned data_length;
	
	// Aguarda ate a chegada do byte indicando o inicio de transmissao do PC ao AVR Master
	while(getchar1()!=SOT);
	// Aguarda o tamanho da mensagem
	data_length = getchar1();
	for (data_index = 0 ; data_index < data_length ; data_index++)
	data[data_index] = getchar1();
	if (getchar1()!=EOT) return (ERRO_GETMSG_GETTING_EOT);
	
	numero_bytes = data_length; // usado para bypass dos comandos da camera HD
	
	return (OK);
}
unsigned get_msg0(unsigned char *data)
{
	unsigned data_index;
	unsigned data_length;
	// Aguarda ate a chegada do byte indicando o inicio de transmissao do PC ao AVR Master
	while(getchar0()!=SOT);
	// Aguarda o tamanho da mensagem
	data_length = getchar0();
	for (data_index = 0 ; data_index < data_length ; data_index++)
	data[data_index] = getchar0();
	if (getchar0()!=EOT) return (ERRO_GETMSG_GETTING_EOT);
	
	numero_bytes = data_length; // usado para bypass dos comandos da camera HD
	
	return (OK);
}

// RS485 com SOT e EOT
unsigned send_485(unsigned data_length, unsigned char *data)
{
	unsigned data_index;

	enable_TX_485_1();
	
	for (data_index = 0 ; data_index < data_length ; data_index++)
	putchar0(data[data_index]);
	
	enable_RX_485_1();
	return (OK);
}
unsigned get_485(unsigned char init, unsigned char *data)
{
	unsigned data_index;
	unsigned data_length;
	
	// Aguarda ate a chegada do byte indicando o inicio de transmissao do PC ao AVR Master
	while(getchar0()!=init);
	// Aguarda o tamanho da mensagem
	data_length = getchar0();
	for (data_index = 0 ; data_index < data_length ; data_index++)
	data[data_index] = getchar1();
	//   if (getchar0()!=EOT) return (ERRO_GETMSG_GETTING_EOT);
	
	numero_bytes = data_length; // usado para bypass dos comandos da camera HD
	
	return (OK);
}
// **********************************************************************
// **********************************************************************

// Functions for Pressure Sensor
// **********************************************************************
// **********************************************************************
void InitVelki(void)
{

	unsigned char SensorInit[4] = {0xfa, 0x30, 0x04, 0x43};
	send_485(4,SensorInit);
}
void ReadVelki(void)
{

	unsigned char ReqPressure1[5] = {0xfa, 0x49, 0x01, 0xa1, 0xa7};
	unsigned char RecPressure1[9];
	//	unsigned char erro[9]={0,0,0,0,1,1,1,1,2};
	uint8_t i;
	
	send_485(5,ReqPressure1);
	//_delay_ms(3);
	
	for(i = 0; i < 9; i++)
	RecPressure1[i] = getchar0();
	
	
	
	//
	// 	// CRC16 recebido do sensor
	// 	uint16_t CRCRec = (RecPressure1[7]<<8) + RecPressure1[8];
	//
	// 	// Calculo de CRC16 dos dados recebidos
	// 	uint16_t CRC = 0xFFFF;
	// 	for(i=0; i < 7; i++)
	// 	CRC = _crc16_update(CRC, RecPressure1[i]);
	//
	// 	if(CRC == CRCRec)
	send_msg_uart1(9,RecPressure1);
	// 	else
	// 		send_msg_uart1(9,RecPressure1);
	// 		//send_msg_uart1(9,erro);//TODO: Adicionar tratamento de erro
}
// **********************************************************************
// **********************************************************************

// Ports configuration
// **********************************************************************
// **********************************************************************
void ROSAPortsConfig(){
	// Port A initialization
	// Func7=Relay08 Func6=Relay07 Func5=Relay06 Func4=Relay05 Func3=Relay04 Func2=Relay03 Func1=Relay02 Func0=Relay01
	//PORTA= 0b11111111;
	//DDRA= 0b11111111;

	// Port B initialization
	// Func7=NC Func6=NC Func5=NC Func4=NC Func3=AD_DOUT Func2=AD_DIN Func1=AD_SCLK Func0=AD_CS#
	PORTB=0b00000000;
	DDRB= 0b00000111;

	// Port C initialization
	// Func7=NC Func6=Eth_Reset Func5=NC Func4=NC Func3=Relay12 Func2=Relay11 Func1=Relay10 Func0=Relay09
// 	PORTC=0b00011111;
// 	DDRC=0b01011111;

	// Port D initialization
	// NC
	PORTD=0x00;
	DDRD=0x00;

	// Port E initialization
	// NC
	PORTE=0x00;
	DDRE=0x00;

	// Port F initialization
	// Func7=NC Func6=Left_Inclinometer Func5=Right_Inclinometer Func4=Key_Inclinometer Func3=ADC_5V Func2=ADC_12V Func1=Base_Inclinometer Func0=ADC_24V
	PORTF=0x00;
	DDRF=0x00;

	// Port G initialization
	// NC
	PORTG=0x00;
	DDRG=0x00;
	
	// Timer/Counter 0 initialization
	// Clock source: System Clock
	// Clock value: Timer 0 Stopped
	// Mode: Normal top=FFh
	// OC0 output: Disconnected
	TCCR0A=0x00;
	TCNT0=0x00;
	OCR0A=0x00;

	// Timer/Counter 1 initialization
	// Clock source: System Clock
	// Clock value: Timer 1 Stopped
	// Mode: Normal top=FFFFh
	// OC1A output: Discon.
	// OC1B output: Discon.
	// OC1C output: Discon.
	// Noise Canceler: Off
	// Input Capture on Falling Edge
	// Timer 1 Overflow Interrupt: Off
	// Input Capture Interrupt: Off
	// Compare A Match Interrupt: Off
	// Compare B Match Interrupt: Off
	// Compare C Match Interrupt: Off
	TCCR1A=0x00;
	TCCR1B=0x00;
	TCNT1H=0x00;
	TCNT1L=0x00;
	ICR1H=0x00;
	ICR1L=0x00;
	OCR1AH=0x00;
	OCR1AL=0x00;
	OCR1BH=0x00;
	OCR1BL=0x00;
	OCR1CH=0x00;
	OCR1CL=0x00;

	// Timer/Counter 2 initialization
	// Clock source: System Clock
	// Clock value: Timer 2 Stopped
	// Mode: Normal top=FFh
	// OC2 output: Disconnected
	ASSR=0x00;
	TCCR2A=0x00;
	TCNT2=0x00;
	OCR2A=0x00;

	// Timer/Counter 3 initialization
	// Clock source: System Clock
	// Clock value: Timer 3 Stopped
	// Mode: Normal top=FFFFh
	// Noise Canceler: Off
	// Input Capture on Falling Edge
	// OC3A output: Discon.
	// OC3B output: Discon.
	// OC3C output: Discon.
	// Timer 3 Overflow Interrupt: Off
	// Input Capture Interrupt: Off
	// Compare A Match Interrupt: Off
	// Compare B Match Interrupt: Off
	// Compare C Match Interrupt: Off
	TCCR3A=0x00;
	TCCR3B=0x00;
	TCNT3H=0x00;
	TCNT3L=0x00;
	ICR3H=0x00;
	ICR3L=0x00;
	OCR3AH=0x00;
	OCR3AL=0x00;
	OCR3BH=0x00;
	OCR3BL=0x00;
	OCR3CH=0x00;
	OCR3CL=0x00;

	// External Interrupt(s) initialization
	// INT0: Off
	// INT1: Off
	// INT2: Off
	// INT3: Off
	// INT4: Off
	// INT5: Off
	// INT6: Off
	// INT7: Off
	EICRA=0x00;
	EICRB=0x00;
	EIMSK=0x00;

	// Timer/Counter 0 Interrupt(s) initialization
	TIMSK0=0x00;
	// Timer/Counter 1 Interrupt(s) initialization
	TIMSK1=0x00;
	// Timer/Counter 2 Interrupt(s) initialization
	TIMSK2=0x00;
	// Timer/Counter 3 Interrupt(s) initialization
	TIMSK3=0x00;

	// 	// USART0 initialization
	// 	// Communication Parameters: 8 Data, 1 Stop, No Parity
	// 	// USART0 Receiver: On
	// 	// USART0 Transmitter: On
	// 	// USART0 Mode: Asynchronous
	// 	// USART0 Baud Rate: 9600
	// 	UCSR0A=0x00;
	// 	UCSR0B=0x18;
	// 	UCSR0C=0x06;
	// 	UBRR0H=0x00;
	// 	UBRR0L=0x33;
	//
	// 	// USART1 initialization
	// 	// Communication Parameters: 8 Data, 1 Stop, No Parity
	// 	// USART1 Receiver: On
	// 	// USART1 Transmitter: On
	// 	// USART1 Mode: Asynchronous
	// 	// USART1 Baud Rate: 9600
	// 	UCSR1A=0x00;
	// 	UCSR1B=0x18;
	// 	UCSR1C=0x06;
	// 	UBRR1H=0x00;
	// 	UBRR1L=0x33;

	// Analog Comparator initialization
	// Analog Comparator: Off
	// Analog Comparator Input Capture by Timer/Counter 1: Off
	ACSR=0x80;
	ADCSRB=0x00;
	//SHT_ConnectionRest(); //T/H sensor
	
	// CAN Controller initialization
	// CAN: Off
	CANGCON=0x00;
	
	// deviceConfig and relays initialization
	configMask1=0xFF; configMask2=0xFE;
	relaysMask1=0xFF; relaysMask2=0xFE;
	
	setAllRelaysROSA();
	/* 	sei();*/
}