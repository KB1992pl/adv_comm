/*
 * 
 * Created: 26.06.2017
 * Author : KB1992PL
 *
 */ 

#ifndef FUNC_H_
#define FUNC_H_

#define F_CPU 16000000L
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD_MODE	1
#define SET_BACK	2
#define SET_FORWARD	3


void uart_init (uint16_t baudrate); 
void uart_transmit (unsigned char data);  //transmit 1 byte
void uart_transmit_string(char text[] ); //transmit char array (string)
void interface(char text[]); //TO DO
uint8_t compare_string(char text1[],const char text2[], uint8_t count, const uint8_t cons_len); //returns 0 when data doesnt match or returns count of compared symbols
void reset_string(volatile char tekst[],volatile uint8_t count); //reset char array
uint8_t find_command(char text[], uint8_t count);
uint16_t value_reader(char text[], uint8_t count);
void uart_transmit_int(uint16_t liczba);



void uart_init (uint16_t baudrate)
{
	baudrate = F_CPU/baudrate/16-1;
	UBRRH=(baudrate>>8);
	UBRRL=baudrate;                         //set baud rate -19200
	UCSRB|=(1<<TXEN)|(1<<RXEN);             //enable receiver and transmitter
	UCSRB |= (1 << RXCIE);					// Enable the USART Recieve Complete interrupt (USART_RXC)
	UBRRH|=(1<<URSEL);
	UCSRC= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);// 8bit data format
	uart_transmit_string("init ok");
}

void uart_transmit (unsigned char data)
{
	while (!( UCSRA & (1<<UDRE)));            // wait for free register
	UDR = data;                               
}


void uart_transmit_string(char text[] )
{
	uint8_t i =0;
	while (text[i] != '\0')
	{
		uart_transmit(text[i]);
		i++;
	}
}

void uart_interface(volatile char text[]){
	if (text =="help")
	{
		uart_transmit_string("help mode");
		text = "";
	}
	else uart_transmit_string(text);
}



uint8_t compare_string(char text1[], const char text2[], uint8_t count, const uint8_t cons_len ){ 
	uint8_t i;
	if(count!=cons_len)
	{
		return 0;
	}
	for (i=0; i<count;i++){
		if (text1[i]!=text2[i]) return 0;
		else if ((text1[i]==0)||(text2[i]==0)) return 0;
	}
	return 1;
}

uint16_t value_reader(char text[], uint8_t count)
{
	uint8_t i = 0;
	uint16_t temp = 0;
	while(i<count)
	{
		if (text[i]<48) //0 w ascii
		{
			return 0;
		}
		else if (text[i]>57) //9 w ascii
		{
			return 0;
		}
		else
		{
			temp =temp*10;
			temp=temp + (text[i]-48);
			i++;
		}
	}
	return temp;
}

void uart_transmit_int(uint16_t number){
	uint8_t status=0;
	uint16_t divider=10000;
	uint8_t temp;
	char ch[5];
	for (uint8_t i=0; i<=4; i++)
	{
		temp = number/divider;
		ch[i] = temp+48;
		number = number - (divider*temp);
		divider = divider/10;
	}
	
	for (uint8_t i = 0; i<=4;i++)
	{
		if (status == 1)
		{
			uart_transmit(ch[i]);
		}
		else if (status == 0)
		{
			if (ch[i]>48) 
			status=1;
			uart_transmit(ch[i]);	
		}
		
	}
}



uint8_t find_command(char tekst[], uint8_t count)
{
	if (compare_string(tekst, "baud", count, 4))
	{
		 return BAUD_MODE;
	}
	else if (compare_string(tekst, "bck", count, 3))
	{
		 return SET_BACK;
	}
	else if (compare_string(tekst, "fwd", count, 3))
	{
		 return SET_FORWARD;
	}
	else uart_transmit_string("compare error");
	
	
}

#endif