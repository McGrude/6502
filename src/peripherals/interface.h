#ifndef INC_6502_INTERFACE_H
#define INC_6502_INTERFACE_H

#include <stddef.h>
#include <stdint.h>

void interface_display_cpu(uint8_t row, uint8_t column);
void interface_display_header(uint8_t row, uint8_t column);
void interface_display_page(uint8_t row, uint8_t column, uint16_t addr);
#endif
