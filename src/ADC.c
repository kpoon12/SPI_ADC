// ADC.c
// This ADC program can set and read ANALOG DEVICES "AD7323BRUZ"
/* Includes ------------------------------------------------------------------*/
// Local include files
#include "ADC.h"
 
static ADC_DEVICE	_adc2ChDevice;
static ADC_DEVICE	_adc4ChDevice; 

void SetAdcChannelAddr(ADC_DEVICE *pDevice, BYTE uChannel)
{	 
	switch (pDevice->uDevice){
		case EXT_ANALOG_INPUT_ADC:
			pDevice->ControlRegister.st2ChAdcCtrlReg.wChannelAddr = uChannel;
			break;
		case ANALOG_INPUT_ADC:
		case CURRENT_SERVO_ADC:
			pDevice->ControlRegister.st4ChAdcCtrlReg.wChannelAddr = uChannel;
			break;
	}
}

WORD GetAdcChannelAddr(ADC_DEVICE *pDevice)
{	 
	WORD wChAddr = 0; 
	
	switch(pDevice->uDevice)
	{
		case EXT_ANALOG_INPUT_ADC:
			wChAddr = pDevice->ResultRegister.st2ChAdcResult.wChannelAddr;
			break;
		case ANALOG_INPUT_ADC:	
		case CURRENT_SERVO_ADC:
			wChAddr = pDevice->ResultRegister.st4ChAdcResult.wChannelAddr;
			break;
	}
	return wChAddr;
}

void SetAdcRangeRegister(ADC_DEVICE *pDevice, BYTE uChannel, WORD wRange)
{
	switch(pDevice->uDevice)
	{
		case EXT_ANALOG_INPUT_ADC:			 
			pDevice->RangeRegister.st2ChRange.wWrite        = ADCWRITE;	
			pDevice->RangeRegister.st2ChRange.wRegSelect    = RANGE_REG;
			pDevice->RangeRegister.st2ChRange.wZero1        = 0;	
			switch (uChannel)
			{
				case CHANNEL_0: pDevice->RangeRegister.st2ChRange.wCh0VoltRange = wRange; break;
				case CHANNEL_1: pDevice->RangeRegister.st2ChRange.wCh1VoltRange = wRange; break;
			}
			break;
		case ANALOG_INPUT_ADC:			
		case CURRENT_SERVO_ADC:
			pDevice->RangeRegister.st4ChRange.wWrite        = ADCWRITE;
			pDevice->RangeRegister.st2ChRange.wRegSelect    = RANGE_REG;
			switch (uChannel)
			{
				case CHANNEL_0: pDevice->RangeRegister.st4ChRange.wCh0VoltRange = wRange; break;
				case CHANNEL_1:	pDevice->RangeRegister.st4ChRange.wCh1VoltRange = wRange; break;
				case CHANNEL_2:	pDevice->RangeRegister.st4ChRange.wCh2VoltRange = wRange; break;
				case CHANNEL_3:	pDevice->RangeRegister.st4ChRange.wCh3VoltRange = wRange; break;
			}
			break;
	}
}

int GetAdcConversionResult(ADC_DEVICE *pDevice)
{
	if (pDevice->uChannel != GetAdcChannelAddr(pDevice))
		return ERROR_UNEXPECTED_CHANNEL;
	
	return ERROR_NONE;
}

int GetAdcData(ADC_DEVICE *pDevice)
{
	int nResultStatus;
	
	/* Set Channel in Control Register */
	pDevice->ControlRegister.wControlRegister |= (ADCWRITE << 15);
	SetAdcChannelAddr(pDevice, pDevice->uChannel);
	
	
	SelectSpiDevice(pDevice->uDevice);	// Assert CS to device 
	pDevice->ResultRegister.wResult = HandshakeSpiWord(pDevice->ControlRegister.wControlRegister);
	SelectSpiDevice(DESELECT_ALL);		// Deassert CS to device 
	
	
	SelectSpiDevice(pDevice->uDevice);   // Assert CS to device

	pDevice->ResultRegister.wResult = (WORD)HandshakeSpiByte(DUMMY_DATA);
	pDevice->ResultRegister.wResult <<= 8;
	pDevice->ResultRegister.wResult |= (WORD)HandshakeSpiByte(DUMMY_DATA);		 

	SelectSpiDevice(DESELECT_ALL);  	// Deselect the ADC Device 
	
	/* Check Results */
	nResultStatus = GetAdcConversionResult(pDevice); 
	return (nResultStatus);
}

void InitAdcControlRegister(ADC_DEVICE *pDevice)
{	
	pDevice->ControlRegister.wControlRegister = (ADCWRITE << 15);
	switch(pDevice->uDevice)
	{
		case EXT_ANALOG_INPUT_ADC:
			pDevice->ControlRegister.st2ChAdcCtrlReg.wRegSelect 	= CTRL_REG;
			pDevice->ControlRegister.st2ChAdcCtrlReg.wChannelAddr 	= pDevice->uChannel;
			pDevice->ControlRegister.st2ChAdcCtrlReg.wConfigMode 	= CM_SINGLE_ENDED;
			pDevice->ControlRegister.st2ChAdcCtrlReg.wPowerMode 	= PM_NORMAL;
			pDevice->ControlRegister.st2ChAdcCtrlReg.wCoding 		= OC_BINARY;
			pDevice->ControlRegister.st2ChAdcCtrlReg.wExtRef 		= USE_EXTERNAL_REF;
			pDevice->ControlRegister.st2ChAdcCtrlReg.wSeq 			= CHANNEL_SEQ_OFF;
			break;
		case ANALOG_INPUT_ADC:
		case CURRENT_SERVO_ADC:
			pDevice->ControlRegister.st4ChAdcCtrlReg.wRegSelect 	= CTRL_REG;
			pDevice->ControlRegister.st4ChAdcCtrlReg.wChannelAddr 	= pDevice->uChannel;
			pDevice->ControlRegister.st4ChAdcCtrlReg.wConfigMode 	= CM_SINGLE_ENDED;
			pDevice->ControlRegister.st4ChAdcCtrlReg.wPowerMode 	= PM_NORMAL;
			pDevice->ControlRegister.st4ChAdcCtrlReg.wCoding 		= OC_BINARY;
			pDevice->ControlRegister.st4ChAdcCtrlReg.wExtRef 		= USE_EXTERNAL_REF;
			pDevice->ControlRegister.st4ChAdcCtrlReg.wSeq 			= CHANNEL_SEQ_OFF;
			break;
	}
}

void InitAdcRangeRegister(ADC_DEVICE *pDevice)
{
	SetAdcRangeRegister(pDevice, CHANNEL_0, MINUSPLUS_10V);
	SetAdcRangeRegister(pDevice, CHANNEL_1, MINUSPLUS_10V);

	if (CURRENT_SERVO_ADC == pDevice->uDevice)
	{
		SetAdcRangeRegister(pDevice, CHANNEL_2, MINUSPLUS_10V);
		SetAdcRangeRegister(pDevice, CHANNEL_3, MINUSPLUS_10V);
	}
}

void InitAdcSeqRegister(ADC_DEVICE *pDevice)
{ 
	pDevice->SeqRegister.wSeqRegister 				= (ADCWRITE << 15);
	pDevice->SeqRegister.stAdcSeqReg.wRegSelect 	= SEQ_REG;
	pDevice->SeqRegister.wSeqRegister 				= 0xE000;
}

int InitAdcDevice(ADC_DEVICE *pDevice)
{
	int nResultStatus;
	
	/* Initialize Range Register */
	InitAdcRangeRegister(pDevice);
	/* Assert CS to device */
	SelectSpiDevice(pDevice->uDevice);
	/* Send Range Register */
	pDevice->ResultRegister.wResult = 
		HandshakeSpiWord(pDevice->RangeRegister.wRangeRegister); 
	/* Deselect the ADC Device */
	SelectSpiDevice(DESELECT_ALL);
	  	
	/*Initialize Control Register */
	InitAdcControlRegister(pDevice);
	/* Assert CS to device */
	SelectSpiDevice(pDevice->uDevice);
	/* Send Control Register */
	pDevice->ResultRegister.wResult = 
		HandshakeSpiWord(pDevice->ControlRegister.wControlRegister); 
	/* Deselect the ADC Device */
	SelectSpiDevice(DESELECT_ALL);
	
	/* Read ADC Value */
	/* Assert CS to device */
	SelectSpiDevice(pDevice->uDevice);
	/* Read high byte from the ADC */			
	pDevice->ResultRegister.wResult = (WORD)HandshakeSpiByte(DUMMY_DATA);
	pDevice->ResultRegister.wResult <<= 8;
	/* Read low byte from ADC */
	pDevice->ResultRegister.wResult |= (WORD)HandshakeSpiByte(DUMMY_DATA);		 
	/* Deselect the ADC Device */
	SelectSpiDevice(DESELECT_ALL);
	
	/* Check Results */
	nResultStatus = GetAdcConversionResult(pDevice);
	return nResultStatus;
}


WORD ReadADC(BYTE uChannel)
{
	WORD wData = 0;
	
		switch(uChannel >> 4)
		{
			//Four channel ADC
			case ANALOG_INPUT_ADC:
			case CURRENT_SERVO_ADC:
				_adc4ChDevice.uChannel = uChannel & ADCDEVICE_MASK;
				_adc4ChDevice.uDevice = uChannel >> 4;
				GetAdcData(&_adc4ChDevice);	 
				wData = _adc4ChDevice.ResultRegister.st4ChAdcResult.wResult;	  
				break;

			//Two channel ADC is default to use
			default:
				_adc2ChDevice.uChannel = uChannel & ADCDEVICE_MASK;
				_adc2ChDevice.uDevice = uChannel >> 4;
				GetAdcData(&_adc2ChDevice);
				wData = _adc2ChDevice.ResultRegister.st2ChAdcResult.wResult;
				break;

		}
		

	return wData >> 1;	 // Convert from 0x1FFF max pos to 0x0FFF 
}

void Init_ADC(void)
{
	/* Deactivate SPI ADC Device Select lines */
	SelectSpiDevice(DESELECT_ALL);
	
	/* Initialize Analog Input Device */
	_adc4ChDevice.bIs4ChDevice = __TRUE;
	_adc4ChDevice.uDevice = ANALOG_INPUT_ADC;
	_adc4ChDevice.uChannel = CHANNEL_0;
	InitAdcDevice(&_adc4ChDevice);
	_adc4ChDevice.uChannel = CHANNEL_1;
	InitAdcDevice(&_adc4ChDevice);
	_adc4ChDevice.uChannel = CHANNEL_2;
	InitAdcDevice(&_adc4ChDevice);
	_adc4ChDevice.uChannel = CHANNEL_3;
	InitAdcDevice(&_adc4ChDevice);
	
	/* Initialize Current Servo Device */
	_adc4ChDevice.bIs4ChDevice = __TRUE;
	_adc4ChDevice.uDevice = CURRENT_SERVO_ADC;
	_adc4ChDevice.uChannel = CHANNEL_0;
	InitAdcDevice(&_adc4ChDevice);
	_adc4ChDevice.uChannel = CHANNEL_1;
	InitAdcDevice(&_adc4ChDevice);
	_adc4ChDevice.uChannel = CHANNEL_2;
	InitAdcDevice(&_adc4ChDevice);
	_adc4ChDevice.uChannel = CHANNEL_3;
	InitAdcDevice(&_adc4ChDevice);

	/* Initialize EXT. ANALOG INPUT Device */
	_adc2ChDevice.bIs4ChDevice = __FALSE;
	_adc2ChDevice.uDevice = EXT_ANALOG_INPUT_ADC;
	_adc2ChDevice.uChannel = CHANNEL_0;
	InitAdcDevice(&_adc2ChDevice);
	_adc2ChDevice.uChannel = CHANNEL_1;
	InitAdcDevice(&_adc2ChDevice);
	InitAdcDevice(&_adc2ChDevice);
}
