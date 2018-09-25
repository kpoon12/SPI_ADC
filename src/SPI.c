/* Includes ------------------------------------------------------------------*/
#include "SPI.h"


/* Private function prototypes -----------------------------------------------*/
void SPI_LowLevel_DeInit(void);
void SPI_LowLevel_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  DeInitializes the peripherals used by the SPI FLASH driver.
  * @param  None
  * @retval None
  */
void SPI_DeInit(void)
{
  SPI_LowLevel_DeInit();
}

/**
  * @brief  Initializes the peripherals used by the SPI FLASH driver.
  * @param  None
  * @retval None
  */
void SPI_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;

  SPI_LowLevel_Init();

  /*!< Deselect the FLASH: Chip Select high */
  SelectSpiDevice(DESELECT_ALL);

  /*!< SPI configuration */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;

  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(spi2_SPI, &SPI_InitStructure);

  /*!< Enable the spi2_SPI  */
  SPI_Cmd(spi2_SPI, ENABLE);
}

/**
  * @brief  Sends a byte through the SPI interface and return the byte received
  *         from the SPI bus.
  * @param  byte: byte to send.
  * @retval The value of the received byte.
  */
uint8_t HandshakeSpiByte(uint8_t byte)
{
  /*!< Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(spi2_SPI, SPI_I2S_FLAG_TXE) == RESET);

  /*!< Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(spi2_SPI, byte);

  /*!< Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(spi2_SPI, SPI_I2S_FLAG_RXNE) == RESET);

  /*!< Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(spi2_SPI);
}

/**
  * @brief  Sends a Half Word through the SPI interface and return the Half Word
  *         received from the SPI bus.
  * @param  HalfWord: Half Word to send.
  * @retval The value of the received Half Word.
  */
uint16_t HandshakeSpiWord(uint16_t data)
{
	uint8_t uSendData;
	uint16_t wDataIn;

	// Send & Recieve High byte
	uSendData = (data & 0xFF00) >> 8;
	wDataIn = HandshakeSpiByte(uSendData);
	wDataIn <<= 8;

	// Send & Recieve Low Byte
	uSendData = (data & 0x00FF) >> 0;
	wDataIn |= HandshakeSpiByte(uSendData);

  return wDataIn;
}


/**
  * @brief  Initializes the peripherals used by the SPI FLASH driver.
  * @param  None
  * @retval None
  */
void SPI_LowLevel_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /*!< Enable the SPI clock */
  spi2_SPI_CLK_INIT(spi2_SPI_CLK, ENABLE);

  /*!< Enable GPIO clocks */
  RCC_AHB1PeriphClockCmd(spi2_SPI_SCK_GPIO_CLK | spi2_SPI_MISO_GPIO_CLK |
                         spi2_SPI_MOSI_GPIO_CLK | adc1_CS_GPIO_CLK | adc2_CS_GPIO_CLK,  ENABLE);

  /*!< SPI pins configuration *************************************************/

  /*!< Connect SPI pins to AF5 */
  GPIO_PinAFConfig(spi2_SPI_SCK_GPIO_PORT, spi2_SPI_SCK_SOURCE, spi2_SPI_SCK_AF);
  GPIO_PinAFConfig(spi2_SPI_MISO_GPIO_PORT, spi2_SPI_MISO_SOURCE, spi2_SPI_MISO_AF);
  GPIO_PinAFConfig(spi2_SPI_MOSI_GPIO_PORT, spi2_SPI_MOSI_SOURCE, spi2_SPI_MOSI_AF);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

  /*!< SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = spi2_SPI_SCK_PIN;
  GPIO_Init(spi2_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /*!< SPI MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  spi2_SPI_MOSI_PIN;
  GPIO_Init(spi2_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /*!< SPI MISO pin configuration */
  GPIO_InitStructure.GPIO_Pin =  spi2_SPI_MISO_PIN;
  GPIO_Init(spi2_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure CS pins in output pushpull mode ********************/
  GPIO_InitStructure.GPIO_Pin =  dac2_CS_PIN | adc2_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(dac2_CS_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  DeInitializes the peripherals used by the SPI FLASH driver.
  * @param  None
  * @retval None
  */
void spi2_LowLevel_DeInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /*!< Disable the spi2_SPI  ************************************************/
  SPI_Cmd(spi2_SPI, DISABLE);

  /*!< DeInitializes the spi2_SPI *******************************************/
  SPI_I2S_DeInit(spi2_SPI);

  /*!< spi2_SPI Periph clock disable ****************************************/
  spi2_SPI_CLK_INIT(spi2_SPI_CLK, DISABLE);

  /*!< Configure all pins used by the SPI as input floating *******************/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

  GPIO_InitStructure.GPIO_Pin = spi2_SPI_SCK_PIN;
  GPIO_Init(spi2_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = spi2_SPI_MISO_PIN;
  GPIO_Init(spi2_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = spi2_SPI_MOSI_PIN;
  GPIO_Init(spi2_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = dac1_CS_PIN;
  GPIO_Init(dac1_CS_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = dac2_CS_PIN;
  GPIO_Init(dac2_CS_GPIO_PORT, &GPIO_InitStructure);
}


/*******************************************************************************
* Function Name  : SelectSpiDevice
* Description    : Commands PLD to assert the proper CS for the SPI device
* Input          : uDevice :  SPI Device to be selected
* Output         : Drives SS0-SS2 lines
* Return         : None
*******************************************************************************/
void SelectSpiDevice(uint8_t uDevice)
{
 	// Diable Select lines
	GPIO_SetBits(adc1_CS_GPIO_PORT, adc1_CS_PIN);
	GPIO_SetBits(adc2_CS_GPIO_PORT, adc2_CS_PIN);

	switch(uDevice)
	{
		case DESELECT_ALL:
 			break;
		case ANALOG_INPUT_ADC:
			GPIO_ResetBits(adc1_CS_GPIO_PORT, adc1_CS_PIN);
			break;
		case CURRENT_SERVO_ADC:
			GPIO_ResetBits(adc2_CS_GPIO_PORT, adc2_CS_PIN);
			break;
		case EXT_ANALOG_INPUT_ADC:
			break;
		default:
			// INVALID SPI DEVICE
			break;
	}
}
