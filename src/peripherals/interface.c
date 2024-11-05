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
  uint16_t local_index = 0;
  uint8_t local_row = row;
  uint8_t local_column = column;

  
  // print page address in upper left corner 
  mvprintw( local_row, local_column, "%04X | ", page);


  local_row = row;
  local_column = column;
  mvprintw( local_row   , local_column+7, "00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F",page);
  mvprintw( local_row+1 , local_column+5, "+-------------------------------------------------");


  local_row = row+2;
  local_column = column+2;
  for ( local_index = 0; local_index < 16; local_index++ ) {
    mvprintw( local_row+local_index , local_column, "%02X |",local_index*16);
  }

  
  local_row = row+2;
  local_column = column+7;
        
  for ( local_index = 0 ; local_index < 256; local_index++ ) {
    // print value at the local_index'th offset into page
    mvprintw(local_row, local_column, "%02X", mp->data[ page + local_index ] );
    // if this is not the first and
    // the next value should be on the following row
    if ( local_index != 0 && (local_index+1) % 16 == 0 ) {
      // increment the row by one
      local_row += 1;
      // and reset the column back to the left
      local_column = column+7;
    } else {
      // otherwise just move over three for next value
      local_column += 3;
    }
  }

  local_column = column + 55;
  local_row = row;

  mvprintw( local_row   , local_column, "|0123456789ABCDEF|");
  mvprintw( local_row+1 , local_column, "+----------------+");
  
  local_row = row + 2;
  for ( int local_index = 0; local_index < 16; local_index ++ ) {
    mvprintw( local_row + local_index , local_column, "|                |");
  }

  
  local_row = row + 2;
  local_column = column + 56;
  
  for ( local_index = 0 ; local_index < 256; local_index++ ) {
    
    if ( ( mp->data[page+local_index] >= 0x20 ) &&  ( mp->data[page+local_index] <= 0x7E ) ) { 
      mvprintw(local_row, local_column, "%c", mp->data[page+local_index]);
    } else {
        mvprintw(local_row, local_column, ".");
    }
    
    if ( local_index != 0 && ((local_index+1) % 16 == 0) ) {
      local_row += 1;
      local_column = column + 56;
    } else {
      local_column += 1;
    }
  }
  
}
