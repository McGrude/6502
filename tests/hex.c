#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

int main ( int argc, char **argv ) {
  int value = 0;
  uint8_t h = 0;
  
  if ( argc != 2 ) { exit(1); }

  value = strtol(argv[1],NULL,0);

  printf("int : %d\n",value);

  h = (uint8_t) value;
  
  printf("hex : %X\n",h);

  exit(0);
}
