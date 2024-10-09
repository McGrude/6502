#include "interface.h"

#include <ncurses.h>
#include <stdint.h>
#include <stdio.h>

#include "../cpu/cpu.h"
#include "../mem/mem.h"

void interface_display_header(void) {
  // mvprintw(1, 3, "6502 Emulator");
  // mvprintw(2, 3, "-----------------------");
  // mvprintw(3, 5, "Commands:");
  // mvprintw(4, 7, "Enter: executes next instruction");
  // mvprintw(5, 7, "r: Resets CPU");
  // mvprintw(6, 7, "q: Quits");
  // mvprintw(7, 3, "-----------------------");
  mvprintw(0,0,"6502 Emulator: Press Keys : Enter to Execute Step, R to Reset, Q to Quit");
}


#define CPU_REG_WIN_X 1
#define CPU_REG_WIN_Y 2
/**
 * interface_display_cpu: prints CPU status to the screen using ncurses
 * @param void
 * @return void
 * */
void interface_display_cpu(void) {
  mvprintw(CPU_REG_WIN_Y  , CPU_REG_WIN_X, "A: 0x%02X", cpu.ac);
  mvprintw(CPU_REG_WIN_Y+1, CPU_REG_WIN_X, "X: 0x%02X", cpu.x);
  mvprintw(CPU_REG_WIN_Y+2, CPU_REG_WIN_X, "Y: 0x%02X", cpu.y );

  mvprintw(CPU_REG_WIN_Y  , CPU_REG_WIN_X+8, "PC: 0x%04X", cpu.pc);
  mvprintw(CPU_REG_WIN_Y+1, CPU_REG_WIN_X+8, "SP: 0x%02X", cpu.sp);
  mvprintw(CPU_REG_WIN_Y+2, CPU_REG_WIN_X+8, "SR: 0x%02X", cpu.sr);
}


#define ZP_WIN_X 10 // 0
#define ZP_WIN_Y 10 // 5
void interface_display_zeropage(void) {
    struct mem* mp = mem_get_ptr();
    mvprintw( ZP_WIN_Y   , ZP_WIN_X, "Zero Page:");
    mvprintw( ZP_WIN_Y+1 , ZP_WIN_X, "     00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F");
    mvprintw( ZP_WIN_Y+2 , ZP_WIN_X, "   +------------------------------------------------");
    mvprintw( ZP_WIN_Y+3 , ZP_WIN_X, "00 : ");
    mvprintw( ZP_WIN_Y+4 , ZP_WIN_X, "10 : ");
    mvprintw( ZP_WIN_Y+5 , ZP_WIN_X, "20 : ");
    mvprintw( ZP_WIN_Y+6 , ZP_WIN_X, "30 : ");
    mvprintw( ZP_WIN_Y+7 , ZP_WIN_X, "40 : ");
    mvprintw( ZP_WIN_Y+8 , ZP_WIN_X, "50 : ");
    mvprintw( ZP_WIN_Y+9 , ZP_WIN_X, "60 : ");
    mvprintw( ZP_WIN_Y+10, ZP_WIN_X, "70 : ");
    mvprintw( ZP_WIN_Y+11, ZP_WIN_X, "80 : ");
    mvprintw( ZP_WIN_Y+12, ZP_WIN_X, "90 : ");
    mvprintw( ZP_WIN_Y+13, ZP_WIN_X, "A0 : ");
    mvprintw( ZP_WIN_Y+14, ZP_WIN_X, "B0 : ");
    mvprintw( ZP_WIN_Y+15, ZP_WIN_X, "C0 : ");
    mvprintw( ZP_WIN_Y+16, ZP_WIN_X, "D0 : ");
    mvprintw( ZP_WIN_Y+17, ZP_WIN_X, "E0 : ");
    mvprintw( ZP_WIN_Y+18, ZP_WIN_X, "F0 : ");

    uint8_t x = ZP_WIN_X+5;
    uint8_t y = ZP_WIN_Y+3;
    for ( uint16_t index = 0 ; index < 256; index++ ) {
      mvprintw(y, x, "%02X", mp->zero_page[index]);
      if ( index != 0 && (index+1) % 16 == 0 ) {
	y += 1;
	x = ZP_WIN_X+5;
      } else {
	x += 3;
      }
    }
    
}


void interface_display_mem(void) {
    struct mem* mp = mem_get_ptr();
    mvprintw(12, 3, "Zero Page:");

    uint8_t x = 3;
    uint8_t y = 14;
    for (uint16_t i = 0; i < 256; i++) {
        mvprintw(y, x, "%02X ", mp->zero_page[i]);
        if (x % WIN_ROWS == 0) {
            y += 1;
            x = 3;
        } else {
            x += 3;
        }
    }

    y += 2;
    mvprintw(y, 3, "Stack:");
    y += 2;
    x = 3;

    for (uint16_t i = 0; i < 256; i++) {
        mvprintw(y, x, "%02X ", mp->stack[i]);
        if (x % WIN_ROWS == 0) {
            y += 1;
            x = 3;
        } else {
            x += 3;
        }
    }

    y += 2;
    mvprintw(y, 3, "Program Data:");
    y += 2;
    x = 3;

    for (uint16_t i = 0x8000 - 0x0200; i < 0x8000 - 0x0200 + 256; i++) {
        mvprintw(y, x, "%02X ", mp->data[i]);
        if (x % WIN_ROWS == 0) {
            y += 1;
            x = 3;
        } else {
            x += 3;
        }
    }

    mvprintw(y + 2, 3, "[...]");
}
