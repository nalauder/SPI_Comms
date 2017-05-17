/*
 * Slave.c
 *
 * Created: 17/05/2017 10:52:13
 * Author : nickl
 */ 
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define BUFFER 80
 
#include <avr/io.h>
#include <stdio.h>
#include "uart.h"

#include <avr/interrupt.h>
#include <util/delay.h>

char buf[100];
volatile unsigned char pos = 0;
volatile char process_it;

void spi_init() {
	//Set PortB 3 (MISO) to output
	DDRB = (1<<4);
	SPCR = (1<<SPE)|(1<<SPIE);
	SPDR = 0;
}


ISR(SPI_STC_vect)
{
	unsigned char c = SPDR;
	if (pos < (sizeof(buf) - 1)) {
		buf[pos++] = c;
	}
	
	if (c == '\n') {
		process_it = 1;
	}
}


int main(void) {
	spi_init();
	
	uart_init();
	stdout = &uart_output;
	stdin  = &uart_input;
	printf("Echo> \r\n");
	
	sei();
    /* Replace with your application code */
    while (1) {
		if (process_it) {
			buf[pos] = '\0';
			printf("%s\r",buf);
			pos = 0;
			process_it = 0;
		}
    } //End of while
}

