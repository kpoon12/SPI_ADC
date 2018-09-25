This SPI_ADC program can set and read ANALOG DEVICES "AD7323BRUZ"

This program is tested with STM32 with SPI connect to two 4 channels ADC and one 2 channels ADC.

How to use the module:

For init

SPI_Init();

Init_ADC();


Read the adc value with ADC chennel

uint16_t value = ReadADC(FB_FOOTCONTROL);

uint16_t value = ReadADC(FB_GAS_MFC);

uint16_t value = ReadADC(FB_TRAVEL);
