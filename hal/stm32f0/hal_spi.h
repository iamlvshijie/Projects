#ifndef __HAL_SPI_H_
#define __HAL_SPI_H_

#include "hal_core.h"
#include "hal_io.h"


typedef enum
{
	spi0 = 0,
	spi1 = 1,
	spi2 = 2
} spi_num;

typedef struct
{
	io_t* tx_io;
	io_cfg_t* tx_io_cfg;
	io_t* rx_io;
	io_cfg_t* rx_io_cfg;
}spi_io_t;

typedef struct
{
	u32_t baud_rate;
	u8_t length;
	u8_t stop_bits;
	u8_t parity;
	u8_t int_mode;
}spi_cfg_t;	 

typedef SPI_TypeDef* spi_t;
//typedef SPI_InitTypeDef spi_cfg_t;

void 		hal_spi_init(spi_num spi_x, spi_cfg_t spi_cfg);
void    hal_spi_deinit(spi_num spi_x);
void 		hal_spi_send_byte(spi_num spi_x,uint8_t data);

uint8_t hal_spi_receive_byte(spi_num spi_x, uint8_t data);

uint8_t hal_spi_chip_select(port_t port, pin_t pin);

uint8_t hal_spi_chip_unselect(port_t port, pin_t pin);

uint8_t hal_spi_write_reg(uint8_t reg,uint8_t value);

uint8_t hal_spi_read_reg(uint8_t reg);

uint8_t hal_spi_read_bytes(uint8_t reg, uint8_t *pbuf, uint8_t bytes);

uint8_t hal_spi_write_bytes(uint8_t reg,uint8_t *pbuf,uint8_t bytes);

#define spi1_irq_handler SPI1_IRQHandler

#endif
