/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_UART_H
#define __HAL_UART_H


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hal_core.h"
#include "hal_io.h"

/* Exported types ------------------------------------------------------------*/

typedef enum
{
	uart0 = 0,
	uart1 = 1,
	uart2 = 2
} uart_num;

typedef struct
{
	io_t* tx_io;
	io_cfg_t* tx_io_cfg;
	io_t* rx_io;
	io_cfg_t* rx_io_cfg;
}uart_io_t;

typedef struct
{
	u32_t baud_rate;
	u8_t length;
	u8_t stop_bits;
	u8_t parity;
	u8_t int_mode;
}uart_cfg_t;	 
	 
/* Exported constants --------------------------------------------------------*/
u32_t hal_uart_deinit(uart_num uart_x);	 
u32_t hal_uart_init(uart_num uart_x,uart_io_t* uart_x_io, uart_cfg_t* uart_cfg); 

u32_t hal_uart_send_byte(uart_num uart_x, u8_t ch);
u32_t hal_uart_receive_byte(uart_num uart_x, u8_t* ch, u32_t timeout);

u32_t hal_uart_send_bytes(uart_num uart_x, u8_t * bytes, u16_t len);
u32_t hal_uart_receive_bytes(uart_num uart_x, u8_t * bytes, u16_t len, u32_t*timeout);

u32_t hal_uart_send_string(uart_num uart_x, u8_t *str);

  
#ifdef __cplusplus
}
#endif

#endif /* __HAL_UART_H */

/*****************************END OF FILE**************/
