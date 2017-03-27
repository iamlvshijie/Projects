#ifndef __HAL_SPI_H_
#define __HAL_SPI_H_

#include "hal_core.h"
#include "hal_io.h"

typedef SPI_TypeDef* spi_t;
typedef SPI_InitTypeDef spi_cfg_t;

void 		hal_spi_init(spi_t spi_x, spi_cfg_t spi_cfg);

void 		hal_spi_send_byte(SPI_TypeDef* SPIx,uint8_t data);

uint8_t hal_spi_receive_byte(SPI_TypeDef* SPIx,uint8_t data);

uint8_t hal_spi_chip_select(port_t port, pin_t pin);

uint8_t hal_spi_chip_unselect(port_t port, pin_t pin);

uint8_t hal_spi_write_reg(uint8_t reg,uint8_t value);

uint8_t hal_spi_read_reg(uint8_t reg);

uint8_t hal_spi_read_bytes(uint8_t reg, uint8_t *pbuf, uint8_t bytes);

uint8_t hal_spi_write_bytes(uint8_t reg,uint8_t *pbuf,uint8_t bytes);

#define spi1_irq_handler SPI1_IRQHandler

#endif
