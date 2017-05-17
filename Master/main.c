/*
 * Master.c
 *
 * Created: 17/05/2017 10:50:51
 * Author : nickl
 */ 
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>


void spi_init(void) {
	// Set MOSI, SS and SCK output, all others input
	DDRB = (1<<DDB5)|(1<<DDB3)|(1<<DDB2);
	PORTB = (1<<DDB2);
	// Enable SPI, Master, set clock rate fck/16
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}


void spi_send(unsigned char data) {
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
}

int main(void) {
	spi_init();
	
	char c;
	
	while(1) {
		PORTB &= ~(1<<DDB2);      // Low, prepare slave
		for (const char * p = "Hello, Nick!\n" ; (c = *p); p++) {
			spi_send(c);
		}
		PORTB |= (1<<DDB2);       // High, Teardown slave
		_delay_ms(100);
	}
}