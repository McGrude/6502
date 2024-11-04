#include "interface.h"

#include <ncurses.h>
#include <stdint.h>
#include <stdio.h>

#include "../cpu/cpu.h"
#include "../mem/mem.h"

void interface_display_header(uint8_t row, uint8_t column) {
  mvprintw(row,column,"6502 Emulator: Press Keys : Enter to Execute Step, R to Reset, Q to Quit");
}


/**
 * interface_display_cpu: prints CPU status to the screen using ncurses
 * @param void
 * @return void
 * */
void interface_display_cpu(uint8_t row, uint8_t column) {
  mvprintw(row  , column, "A: 0x%02X", cpu.ac);
  mvprintw(row+1, column, "X: 0x%02X", cpu.x);
  mvprintw(row+2, column, "Y: 0x%02X", cpu.y );

  mvprintw(row  , column+8, "PC: 0x%04X", cpu.pc);
  mvprintw(row+1, column+8, "SP: 0x%02X", cpu.sp);
  mvprintw(row+2, column+8, "SR: 0x%02X", cpu.sr);
}


void interface_display_page(uint8_t row, uint8_t column, uint16_t addr) {
  struct mem* mp = mem_get_ptr();

  uint16_t page = (addr) & (uint16_t)0xFF00;
  
  mvprintw( row+1 , column, "%04X | 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F",page);
  mvprintw( row+2 , column, "     +-------------------------------------------------");
  mvprintw( row+3 , column, "  00 | ");
  mvprintw( row+4 , column, "  10 | ");
  mvprintw( row+5 , column, "  20 | ");
  mvprintw( row+6 , column, "  30 | ");
  mvprintw( row+7 , column, "  40 | ");
  mvprintw( row+8 , column, "  50 | ");
  mvprintw( row+9 , column, "  60 | ");
  mvprintw( row+10, column, "  70 | ");
  mvprintw( row+11, column, "  80 | ");
  mvprintw( row+12, column, "  90 | ");
  mvprintw( row+13, column, "  A0 | ");
  mvprintw( row+14, column, "  B0 | ");
  mvprintw( row+15, column, "  C0 | ");
  mvprintw( row+16, column, "  D0 | ");
  mvprintw( row+17, column, "  E0 | ");
  mvprintw( row+18, column, "  F0 | ");
  
  uint8_t y = row+3;
  uint8_t x = column+7;
        
  for ( uint16_t index = 0 ; index < 256; index++ ) {
    mvprintw(y, x, "%02X", mp->data[page+index]);
    if ( index != 0 && (index+1) % 16 == 0 ) {
      y += 1;
      x = column+7;
    } else {
      x += 3;
    }
  }

  column = column+55;
  mvprintw( row+1 , column, " 0123456789ABCDEF");
  mvprintw( row+2 , column, "+----------------+");
  
  for ( int i = 3; i <= 18; i ++ ) {
    mvprintw( row+i , column, "|                |");
  }

  y = row+3;
  x = column+1;
  for ( uint16_t index = 0 ; index < 256; index++ ) {

    if ( ( mp->data[page+index] >= 0x20 ) &&  ( mp->data[page+index] <= 0x7E ) ) { 
      mvprintw(y, x, "%c", mp->data[page+index]);
    } else {
      mvprintw(y, x, ".");
    }

    
    if ( index != 0 && ((index+1) % 16 == 0) ) {
      y += 1;
      x = column+1;
    } else {
      x += 1;
    }
  }
  
  
  

  
}

/*
  
          1111111111222222222233333333334444444444555555555566666666667777777777
01234567890123456789012345678901234567890123456789012345678901234567890123456789
  
 0000 | 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F  0123456789ABCDEF 
      +-------------------------------------------------+----------------+
   00 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |                |
   10 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |                |
   20 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |                |
   30 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |                |
   40 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |                |
   50 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |                |
   60 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |                |
   70 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |                |
   80 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |                |
   90 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |                |
   A0 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |                |
   B0 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |                |
   C0 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |                |
   D0 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |                |
   E0 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |                |
   F0 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |                |
   
*/
