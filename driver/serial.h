#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>
#include "hal_uart.h"

typedef struct{
	uart_num uart;
	uart_io_t *uart_io;
  uart_cfg_t *uart_cfg;
}serial_cfg_t;

void console_init(serial_cfg_t * cfg);
void serial_init(serial_cfg_t* serial_cfg);

#endif
