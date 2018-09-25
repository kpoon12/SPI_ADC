// ADC.h

#ifndef _ADC_H
#define _ADC_H

#include "spi.h"

 
//-----------------------------------// 
// Generic Defines                    //
//-----------------------------------//
#define ADCDEVICE_MASK	        0x07
#define ADCDATA_MASK		  0x0FFF
#define ADC_DEVICES			    0x04
#define ADC_XFER_LENGTH		    0x02

// Register Selection  
#define CTRL_REG		        0x00
#define RANGE_REG		        0x01
#define SEQ_REG					0x03
 
#define ADCREAD					0x00
#define ADCWRITE			    0x01

//---------------------------------//
// Control Register Configuration  //
//---------------------------------//

// Channel Sequencer Selection
#define CHANNEL_SEQ_OFF        0x00
#define CHANNEL_SEQ_PREV       0x01
#define CHANNEL_SEQ_ON         0x02

// Power Mode Selection
#define PM_NORMAL		       0x00
#define PM_AUTOSTANDBY	       0x01
#define PM_AUTOSHUTDOWN        0x02
#define PM_FULLSHUTDOWN        0x03

// Analog Input Configuration Mode
#define CM_SINGLE_ENDED        0x00
#define CM_PSEUDO_DIFF         0x01
#define CM_FULLY_DIFF          0x02
#define CM_3PSEUDO_DIFF        0x03
#define CM_INVALID		       0x03

// Ouput Coding type
#define OC_TWOSCOMPLEMENT      0x00
#define OC_BINARY              0x01

// Reference Voltage Used
#define USE_EXTERNAL_REF       0x00
#define USE_INTERNAL_REF       0x01

// Channel Address Bit for next conversion
#define CHANNEL_0		       0x00
#define CHANNEL_1		       0x01
#define CHANNEL_2		       0x02
#define CHANNEL_3              0x03


// Adress for difference ADC
#define ANALOG_INPUT_ADC		0x01 // SPI Select ADC Slave Devices
#define CURRENT_SERVO_ADC		0x02 // SPI Select ADC Slave Devices
#define EXT_ANALOG_INPUT_ADC	0x03 // SPI Select ADC Slave Devices

#define DUMMY_DATA				0x0000						//Read convert data

// ADC Channels
#define	FB_FOOTCONTROL		((ANALOG_INPUT_ADC << 4)	 | CHANNEL_0) // (0x01 << 4) | 0x01 = 0x10
#define FB_EXT_STEERING_IN  ((ANALOG_INPUT_ADC << 4)	 | CHANNEL_1) // (0x01 << 4) | 0x00 = 0x11
#define FB_SPARE_IN1  		((ANALOG_INPUT_ADC << 4)	 | CHANNEL_2) // (0x01 << 4) | 0x00 = 0x12
#define FB_SPARE_IN2  		((ANALOG_INPUT_ADC << 4)	 | CHANNEL_3) // (0x01 << 4) | 0x00 = 0x13

#define	FB_CURRENT			((CURRENT_SERVO_ADC << 4)	 | CHANNEL_0) // (0x02 << 4) | 0x00 = 0x20
#define	FB_GAS_MFC			((CURRENT_SERVO_ADC << 4)	 | CHANNEL_1) // (0x02 << 4) | 0x01 = 0x21
#define	FB_SETPOT			((CURRENT_SERVO_ADC << 4)	 | CHANNEL_2) // (0x02 << 4) | 0x02 = 0x22
#define	FB_PS_VOLTAGE		((CURRENT_SERVO_ADC << 4)    | CHANNEL_3) // (0x02 << 4) | 0x00 = 0x23
 
#define	FB_TRAVEL			((EXT_ANALOG_INPUT_ADC << 4)	 | CHANNEL_0) // (0x03 << 4) | 0x00 = 0x20
#define	FB_WIREFEED			((EXT_ANALOG_INPUT_ADC << 4)	 | CHANNEL_1) // (0x03 << 4) | 0x01 = 0x21


typedef union CONTROL_REGISTER
{
	WORD wControlRegister;
	struct _st2ChAdcCtrlReg
	{
		WORD wReserved:1;
		WORD wZero1:1;
		WORD wSeq:2;
		WORD wExtRef:1;
		WORD wCoding:1;
		WORD wPowerMode:2;
		WORD wConfigMode:2;
		WORD wChannelAddr:1;
		WORD wZero2:2;
		WORD wRegSelect:1;
		WORD wZero3:1;
		WORD wWrite:1;
	} st2ChAdcCtrlReg;

	struct _st4ChAdcCtrlReg
	{
		WORD wReserved:1;
		WORD wZero1:1;
		WORD wSeq:2;
		WORD wExtRef:1;
		WORD wCoding:1;
		WORD wPowerMode:2;
		WORD wConfigMode:2;
		WORD wChannelAddr:2;
		WORD wZero2:1;
		WORD wRegSelect:2;
		WORD wWrite:1;	
	} st4ChAdcCtrlReg;
} CONTROL_REGISTER;


//---------------------------------//
// Range Register Configuration    //
//---------------------------------//

// Voltage Range Used
#define MINUSPLUS_10V       0x0000
#define MINUSPLUS_5V        0x0001
#define MINUSPLUS_2V        0x0002
#define PLUS_10V	        0x0003

typedef union RANGE_REGISTER
{
	WORD wRangeRegister;
	struct _st2ChRange
	{
		WORD wReserved1:7;
		WORD wCh1VoltRange:2; 
		WORD wReserved:2;
		WORD wCh0VoltRange:2;
		WORD wRegSelect:1;
		WORD wZero1:1;
		WORD wWrite:1;	
	} st2ChRange;

	struct _st4ChRange
	{
		WORD wReserved1:5;
		WORD wCh3VoltRange:2;
		WORD wCh2VoltRange:2; 
		WORD wCh1VoltRange:2;
		WORD wCh0VoltRange:2;
		WORD wRegSelect:2;
		WORD wWrite:1;
	} st4ChRange;
} RANGE_REGISTER;

//---------------------------------//
// Sequence Register Configuration    //
//---------------------------------//
 
typedef union SEQ_REGISTER
{
	WORD wSeqRegister;
	struct _stAdcSeqReg
	{
		WORD wReserved1:9;
		WORD wCh3SeqEnable:1; 
		WORD wCh2SeqEnable:1;
		WORD wCh1SeqEnable:1;
		WORD wCh0SeqEnable:1;
		WORD wRegSelect:2;
		WORD wWrite:1;	
	} stAdcSeqReg;
} SEQ_REGISTER;


typedef union RESULT_REGISTER
{
	WORD wResult;
	struct _st2ChAdcResult
	{
		WORD wResult:13;
		WORD wChannelAddr:1;
		WORD wZero1:2;	
	} st2ChAdcResult;

	struct _st4ChAdcResult
	{
		WORD wResult:13;
		WORD wChannelAddr:2;
		WORD wZero1:1;		
	} st4ChAdcResult;
} RESULT_REGISTER;

typedef struct {
	BYTE			  uDevice;
	BYTE			  uChannel;
	BYTE   			  bIs4ChDevice;
	CONTROL_REGISTER  ControlRegister;
	RANGE_REGISTER    RangeRegister;
	SEQ_REGISTER	  SeqRegister;
	RESULT_REGISTER	  ResultRegister;
} ADC_DEVICE;


//-------------------------------------------//
// Definitions that represent error codes.   //
//-------------------------------------------//
 
 // general
#define ERROR_NONE                                   0
#define ERROR_INVALID_ERROR_CODE                    -1
#define ERROR_NULL_POINTER                          -2
#define ERROR_BUFFER_TOO_SMALL                      -3
// initialization

// result handling
#define ERROR_UNEXPECTED_CHANNEL                   -10
#define	ERROR_UNEXPECTED_DATA					   -11
  
 
// device to int
#define ERROR_NO_DEVICE_STRING                      -20
#define ERROR_UNKNOWN_DEVICE                        -21
 
void Init_ADC(void);
WORD ReadADC(BYTE uChannel);
int InitAdcDevice(ADC_DEVICE *pAdcDevice);
int GetAdcData(ADC_DEVICE *pAdcDevice);

#endif // _ADC_H

