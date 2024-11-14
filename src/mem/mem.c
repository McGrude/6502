#include "mem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "../utils/misc.h"

/**
 * The memory:
 *
 *  - RESERVED: 256 bytes 0x0000 to 0x00FF -> Zero Page
 *  - RESERVED: 256 bytes 0x0100 to 0x01FF -> System Stack
 *  - PROGRAM DATA: 0x10000 - 0x206
 *  - RESERVED: last 6 bytes of memory
 *
 *  pages are split into different arrays
 *
 * */
struct mem memory;

/**
 * load_program: Loads binary into program data memory
 * @param path Path to binary on hosst machine
 * @return void
 * */
void load_program(uint16_t address, char* path) {
  FILE* fp = fopen(path, "rb");
  
  if (fp == NULL) {
    fprintf(stderr, "[FAILED] Error while loading provided file.\n");
    exit(1);
  }
  
  struct stat st;
  stat(path, &st);
  size_t fsize = st.st_size;
  
  size_t bytes_read = fread(memory.data + (address), 1, sizeof(memory.data), fp);

  
  
  if (bytes_read != fsize) {
    fprintf( stderr, "[FAILED] Amount of bytes read doesn't match read file size.\n");
    exit(1);
  }
  
  fclose(fp);
}

/**
 * mem_init: Initialize the memory to its initial state
 *
 * @param void
 * @return void
 * */
void mem_init(void) {
  memset(memory.data, 0, sizeof(memory.data));
  // The 6502 reset vector is stored at 0xFFFC and 0xFFFD.  The CPU
  // jumps to the address stored there at reset.
  
  // Store 0x8000 at 6502 reset vector.  We'll remove later when
  // we have more binary loading in place.  We'll load a "rom"
  // in that space and it will store the reset vector @0xFFFC
  
  memory.data[0xFFFC] = 0x00;
  memory.data[0xFFFD] = 0x80;
  
}

/**
 * mem_get_ptr: returns pointer to currently active memory struct
 * */
struct mem* mem_get_ptr(void) {
  struct mem* mp = &memory;
  return mp;
}

/**
 * mem_dump: Dumps the memory to a file called dump.bin
 *
 * @param void
 * @return 0 if success, 1 if fail
 * */
int mem_dump(void) {
  FILE* fp = fopen("dump.bin", "wb+");
  if (fp == NULL) return 1;
  
  size_t wb = fwrite(memory.data, 1, sizeof(memory.data), fp);
  if (wb != sizeof(memory.data)) {
    printf("[FAILED] Errors while dumping the program data.\n");
    fclose(fp);
    return 1;
  }
  
  fclose(fp);
  return 0;
}
