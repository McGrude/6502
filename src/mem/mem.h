#ifndef INC_6502_MEM_H
#define INC_6502_MEM_H

#include <stddef.h>
#include <stdint.h>

#define TOTAL_MEM 1024 * 64

struct mem {
  uint8_t data[TOTAL_MEM];
};

void mem_init(char* filename);
int mem_dump(void);
struct mem* mem_get_ptr(void);

#endif
