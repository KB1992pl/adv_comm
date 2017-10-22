/*
 * 
 * Created: 26.06.2017 18:07:09
 * Author : KB1992PL
 *
 *
 */ 
#define F_CPU 16000000L
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "func.h"


void reset_string(volatile char text[], volatile uint8_t count);

volatile uint16_t received_value=0;
volatile uint8_t current_mode=0;
volatile char received[20]="";
volatile int8_t global_i = 0;

int main(void)
{
	DDRA = 0xff;
	_delay_ms(100);		
	uart_init(19200);
	sei(); 
    while (1) 
    {
		
}
}


ISR(USART_RXC_vect) //interrupt after receiving byte
{	
	char temp;
	temp = UDR; //fetch the received byte value

		if (temp =='!')
		{
			current_mode = find_command(received, global_i);
			switch(current_mode)
			{
				case BAUD_MODE:
				uart_transmit_string("baud value? ");
				break;
				
				case  SET_BACK:
				uart_transmit_string("back speed? ");
				break;
				
				case SET_FORWARD:
				uart_transmit_string("forward speed? ");
				break;
				
				default:
				uart_transmit_string("!unknown command! ");
				break;
			}
			
			
			reset_string(received,global_i);
			global_i=0;
			return;
			
		}
		
		if (temp=='@')
		{
			uart_transmit_string("reading value ");
			received_value=value_reader(received,global_i);
			reset_string(received,global_i);
			if (received_value!=0)
			{
				switch(current_mode)
				{
					
					case BAUD_MODE:
					uart_init(received_value);
					break;
					
					case SET_BACK:
					PORTA=1;
					break;
					
					case SET_FORWARD:
					PORTA=2;
					break;
					
					default:
					break;
					
				}
				
			}
			reset_string(received,global_i);
			global_i=0;
			return;

		}
		
		
		received[global_i] =temp;
		global_i++;
		if (global_i==18) {
			reset_string(received,global_i);
			global_i=0;
		}
}


void reset_string(volatile char tekst[], volatile uint8_t count){
	count++;
	while (count!=0){
		count--;
		tekst[count]=0x0;
	}
}

