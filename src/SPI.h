/*
 * SPI.h
 *
 */
/* Includes ------------------------------------------------------------------*/
#include "global_includes.h"

#ifndef CORE_VXD_SPI_H_
#define CORE_VXD_SPI_H_

/* SPI Interface pins  */
#define spi2_SPI                           SPI2
#define spi2_SPI_CLK                       RCC_APB1Periph_SPI2
#define spi2_SPI_CLK_INIT                  RCC_APB1PeriphClockCmd

#define spi2_SPI_SCK_PIN                   GPIO_Pin_1
#define spi2_SPI_SCK_GPIO_PORT             GPIOI
#define spi2_SPI_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOI
#define spi2_SPI_SCK_SOURCE                GPIO_PinSource1
#define spi2_SPI_SCK_AF                    GPIO_AF_SPI2

#define spi2_SPI_MISO_PIN                  GPIO_Pin_2
#define spi2_SPI_MISO_GPIO_PORT            GPIOI
#define spi2_SPI_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOI
#define spi2_SPI_MISO_SOURCE               GPIO_PinSource2
#define spi2_SPI_MISO_AF                   GPIO_AF_SPI2

#define spi2_SPI_MOSI_PIN                  GPIO_Pin_3
#define spi2_SPI_MOSI_GPIO_PORT            GPIOI
#define spi2_SPI_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOI
#define spi2_SPI_MOSI_SOURCE               GPIO_PinSource3
#define spi2_SPI_MOSI_AF                   GPIO_AF_SPI2

//define CS for adc chip 1
#define adc1_CS_PIN                        GPIO_Pin_8
#define adc1_CS_GPIO_PORT                  GPIOF
#define adc1_CS_GPIO_CLK                   RCC_AHB1Periph_GPIOF

//define CS for adc chip 2
#define adc2_CS_PIN                        GPIO_Pin_9
#define adc2_CS_GPIO_PORT                  GPIOF
#define adc2_CS_GPIO_CLK                   RCC_AHB1Periph_GPIOF



/* Public function prototypes -----------------------------------------------*/
void SPI_Init(void);
void SPI_DeInit(void);

/* Low layer functions */
uint8_t HandshakeSpiByte(uint8_t byte);
uint16_t HandshakeSpiWord(uint16_t HalfWord);
void SelectSpiDevice(uint8_t uDevice);

#endif /* CORE_VXD_SPI_H_ */
