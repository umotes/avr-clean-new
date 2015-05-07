/*
 * Copyright (c) 2010, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

/**
 * \file
 *         Sensor driver for reading the temperature sensor in the Atmel Xplained Pro Board
 * \author
 *         Dag Björklund <dag@iki.fi>
 */

#include "dev/temperature-sensor.h"
#include "contiki.h"
#include <util/twi.h>
#include "dev/twiDriver.h"



#define TEMP_LOWLMT_ADDR 0x12
#define TEMP_HILMT_ADDR  0x13
#define TEMP_CONFIG_ADDR 0x01
#define DEF_POINTER_ADDR 0x00

//#define TEMP_CONV_RESO      0x
//#define TEMP_ALERT_PIN      0x

#define SENSOR_CONFIG_HT  1
#define SENSOR_CONFIG_LT  2
#define SENSOR_CONFIG_RESO 3
#define SENSOR_CONFIG_ALERT 4




const struct sensors_sensor temperature_sensor;

/*---------------------------------------------------------------------------*/
static int
value(int type)
{
  return ReadTempVal();
} 
/*---------------------------------------------------------------------------*/

static int configure(int type, int c)
{
   int value=c;
   switch(type)
   {
   
    case SENSORS_HW_INIT:
    TWIInit();
    return 0;
 
    case SENSOR_CONFIG_LT:
        if(value)
        {
	return SetTempLowLimit(value);
        return 0;      
	}
	else 
	return 0;
	

    case SENSOR_CONFIG_HT:
        
	if(value)
	{
	return SetTempHiLimit(value);
        return 0;
	}
	
        else 
	
	return 0;
       
 	
	case SENSOR_CONFIG_RESO:
        return 0;
       
	case SENSOR_CONFIG_ALERT:
        return 0;
  }
return 0;
}

/*---------------------------------------------------------------------------*/
static int
status(int type)
{
  return 0;
}

/*---------------------------------------------------------------------------*/
SENSORS_SENSOR(temperature_sensor, TEMPERATURE_SENSOR,
               value, configure, status);

/*---------------------------------------------------------------------------*/
//   Read Sensor Temperature 
/*---------------------------------------------------------------------------*/ 
int16_t ReadTempVal(void)
{
  //  uint8_t adressRead = b10010011;
  uint8_t slaveAddr = 0b1001011; //   0b10010001; 
  uint8_t temp;
  uint16_t tempData = 0;
  TWIInit();

  TWIStart();
  
  if ((TWIGetStatus() != TW_START) && (TWIGetStatus() != TW_REP_START))
    return 1;

  //select devise and send A2 A1 A0 address bits
  TWIWrite( (slaveAddr<<1) | TWI_READ );

  if ((TWIGetStatus() != TW_MR_SLA_ACK) && (TWIGetStatus() != TW_MT_SLA_ACK))
    return 2;

  // read first byte
  temp = TWIReadACK();

  if (TWIGetStatus() != TW_MR_DATA_ACK)
    return 3;

  tempData += temp;
  tempData = tempData << 8;
  // read second byte
  temp = TWIReadNACK();
  if (TWIGetStatus() != TW_MR_DATA_ACK)
    return 4;
  
  tempData += temp;  
  
  TWIStop();

  // 128 - (2^11-t>>4)*0.0625
//  return (int)(128 - (2048-(tempData>>4))*0.0625);
    return tempData;
}

float ReadTemperature(void)
{
int16_t rawadc_val=0;
int16_t sign=1;
int16_t abstemp, temp_fraction,temp_integer; 

rawadc_val=Readtempval();

if(rawadc_val&0x8000)
 {
 abstemp=(rawadc_val^0xFFFF)+1;
 sign= -1;
}
else 
{
abstemp=rawadc_val;
}

temp_integer=(abstemp>>8)*sign;

temp_fraction=((abstemp>>4)%16)*625;

//return  value  Pending -- need to check 

}





/*---------------------------------------------------------------------------*/
//   Configure Temperature Low Limit Register
/*---------------------------------------------------------------------------*/
int8_t SetTempLowLimit(uint16_t value)
{

// Temp Low Limit Configuratuation register sub address
  uint8_t loLmtSubAddr = TEMP_LOWLMT_ADDR; 
  uint8_t slaveAddr = 0b1001011; //   0b10010001; 
  uint16_t temp;
  TWIInit();

  TWIStart();

  if (TWIGetStatus() != TW_START)
    return 1;

  //select devise and send A2 A1 A0 address bits
  TWIWrite( (slaveAddr<<1) | TWI_WRITE );

  if (TWIGetStatus() != TW_MR_SLA_ACK)
    return 2;

  // Write Address Pointer with Low Temperature Configuration address
  TWIWrite(loLmtSubAddr);

  if (TWIGetStatus() != TW_MR_SLA_ACK)
    return 3;

  // Set Low Limit - Pass value to  be written  to Low Limit Re4gister
  // Write  High Byte of the value Passes  and then Wait for  ACK 
  // After ACK  Write the  Low Byte  and then  Wait  for ACK  for Succesful Write
  // FOllowed  by I2C/TWI  Stop  COmmand, as per  Data Sheet of Sensor 


  temp=(uint8_t)value;
  TWIWrite(temp);
  if (TWIGetStatus() != TW_MR_SLA_ACK)
    return 4;

  temp=(uint8_t)(value<<8);
  TWIWrite(temp);
  if (TWIGetStatus() != TW_MR_SLA_ACK)
    return 5;

  TWIStop();
    return 6;

}

/*---------------------------------------------------------------------------*/
//   Configure Temperature High Limit Register
/*---------------------------------------------------------------------------*/
int8_t SetTempHiLimit(uint16_t value)
{
// Temp Low Limit Configuratuation register sub address
  uint8_t hiLmtSubAddr = TEMP_HILMT_ADDR;
  uint8_t slaveAddr = 0b1001011; //   0b10010001; 
  uint8_t temp;

TWIInit();

  TWIStart();

  if (TWIGetStatus() != TW_START)
    return 1;

  //select devise and send A2 A1 A0 address bits
  TWIWrite( (slaveAddr<<1) | TWI_WRITE );

  if (TWIGetStatus() != TW_MR_SLA_ACK)
    return 2;

  // Write Address Pointer with High Temperature Limit Configuration address
  TWIWrite(hiLmtSubAddr);

  if (TWIGetStatus() != TW_MR_SLA_ACK)
    return 3;

  // Set High Temperature  Limit - Pass value to  be written  to Low Limit Re4gister
  temp= (uint8_t)value;
  TWIWrite(temp);

  if (TWIGetStatus() != TW_MR_SLA_ACK)
    return 4;

  temp=(uint8_t)(value<<8);
 TWIWrite(temp);

  if (TWIGetStatus() != TW_MR_SLA_ACK)
    return 5;


  TWIStop();
  return 6;


}

/*---------------------------------------------------------------------------*/
//   Reset Address Pointer with Default Address After Writing Configuration Register
/*---------------------------------------------------------------------------*/
int8_t ResetTempSenPtr(uint8_t slaveAddr)
{
  uint8_t defaultAddr =DEF_POINTER_ADDR;
 // uint8_t slaveAddr = 0b1001011; //   0b10010001; 
  TWIInit();

  TWIStart();

  if (TWIGetStatus() != TW_START)
    return 1;

  //select devise and send A2 A1 A0 address bits
  TWIWrite( (slaveAddr<<1) | TWI_WRITE );

  if (TWIGetStatus() != TW_MR_SLA_ACK)
    return 2;

  
  TWIWrite(defaultAddr);
  if (TWIGetStatus() != TW_MR_DATA_ACK)
    return 3;

  TWIStop();
  return 4;

}

