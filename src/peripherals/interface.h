#ifndef INC_6502_INTERFACE_H
#define INC_6502_INTERFACE_H

#include <stddef.h>
#include <stdint.h>

#define WIN_ROWS 35
#define WIN_COLS 50

void interface_display_cpu(void);
void interface_display_mem(void);
void interface_display_zeropage(void);
void interface_display_header(void);
void interface_display_page(uint8_t y_loc, uint8_t x_loc, uint16_t addr);

#endif
