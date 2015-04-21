#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "adc/adc.h"

#define UART_BAUD_RATE 2400
#include "uart/uart.h"

#include "temt6000/temt6000.h"

int main(void) {
	char printbuff[100];
	uint16_t adc = 0;

	//init uart
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );

	//init adc
	adc_init();

	//init interrupt
	sei();

	for (;;) {
		uart_puts("reading...\r\n");

		//get adc reference voltage
		double adcvref = acd_getrealvref();

		//get adc
		adc = adc_read(0);
		long res = adc_getresistence(adc, 10000);
		double vol = adc_getvoltage(adc, adcvref);

		//get lux
		double lux = temt6000_getlux(vol);

		itoa(adc, printbuff, 10);
		uart_puts("ADC     "); uart_puts(printbuff); uart_puts("\r\n");

		ltoa(res, printbuff, 10);
		uart_puts("RES     "); uart_puts(printbuff); uart_puts("\r\n");

		dtostrf(vol, 3, 5, printbuff);
		uart_puts("VOL     "); uart_puts(printbuff); uart_puts("\r\n");

		dtostrf(lux, 3, 5, printbuff);
		uart_puts("lux     "); uart_puts(printbuff); uart_puts("\r\n");

		uart_puts("\r\n");

		_delay_ms(1000);
	}
	
	return 0;
}
