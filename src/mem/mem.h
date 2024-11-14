#ifndef INC_6502_MEM_H
#define INC_6502_MEM_H

#include <stddef.h>
#include <stdint.h>

#define TOTAL_MEM 1024 * 64

struct mem {
  uint8_t data[TOTAL_MEM];
};

void mem_init(void);
int mem_dump(void);
void load_program(uint16_t address, char* filename);
		  
struct mem* mem_get_ptr(void);

#endif
