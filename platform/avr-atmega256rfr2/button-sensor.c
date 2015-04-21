/* Dummy sensor routine */

#include "lib/sensors.h"
#include "dev/button-sensor.h"
const struct sensors_sensor button_sensor;
static int status(int type);
struct sensors_sensor *sensors[1];
unsigned char sensors_flags[1];


static int
value(int type)
{
 return 0;
}

static int
configure(int type, int c)
{
	switch (type) {
	case SENSORS_ACTIVE:
		if (c) {
			if(!status(SENSORS_ACTIVE)) {
			}
		} else {
		}
		return 1;
	}
	return 0;
}

static int
status(int type)
{
	switch (type) {
	case SENSORS_ACTIVE:
	case SENSORS_READY:
		return 1;
	}
	return 0;
}

SENSORS_SENSOR(button_sensor, BUTTON_SENSOR,
	       value, configure, status);


//==Interrupt  Based  Pin Status Check for test. better than Polloing  But  there are  chances of False Triggering 


void congigure_button_pin(void)
{

USER_SW_DIR & = ~(1<<PB4);  // Set the Pin as Input  
USER_SW_PORT |= (1<<PB4);  // When Pin is set as Input and PORT Pin is Pulled high by Writing it do not actually drive the Pin High , it 
                           // only enbles the Pull up resistro
DDRE |= (1<<PE4);
cli(); // Disable Interupt before making changes to Interrupt setup  registers
PCMSK0 |= (1<<PCINT4);  //Enable Enterrupt for any change in PB4  Pin State 
PCICR |=  (1<PCIE0);
sie();


}


ISR (PCINT0_vect) 
{


PORTE ^ = (1<<PE4);
}














