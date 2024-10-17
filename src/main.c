#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include "cpu/cpu.h"
#include "mem/mem.h"
#include "peripherals/interface.h"
#include "peripherals/kinput.h"

uint8_t DEBUG = 0;

int main(int argc, char** argv) {
    switch (argc) {
    case 1:
      mem_init("example.bin");
      break;
      
    case 2:
      mem_init(argv[1]);
      break;
      
    default:
      fprintf(stderr, "[FAILED] Too many arguments.\n");
      exit(1);
    }

    WINDOW* win = newwin(WIN_ROWS, WIN_COLS, 0, 0);
    if ((win = initscr()) == NULL) {
      fprintf(stderr, "[FAILED] Error initialising ncurses.\n");
      exit(1);
    }
    
    uint32_t rows,cols;
    getmaxyx(win,rows,cols);
    //mvprintw(1,0,"W:%d H:%d",cols,rows);
    if ( cols < 132 || rows < 50) {
      delwin(win);
      endwin();
      fprintf(stderr, "\n\n[FAILED] Terminal Size less than 132x50.\n");
      exit(1);
    }

    if (has_colors() == FALSE) {
      endwin();
      fprintf(stderr, "\n\n[FAILED] Your terminal does not support color\n");
      exit(1);
    }
    
    start_color();
    init_pair(1, COLOR_WHITE,COLOR_BLACK);
   
    curs_set(0);
    noecho();
    wrefresh(win);
    interface_display_header();
    wrefresh(win);

    cpu_init();
    cpu_reset();
    
    do {
      interface_display_cpu();
      interface_display_page(7,1,0x0000);   // zero_page
      interface_display_page(7,76,0x0100);  // stack
      interface_display_page(26,1,0x8000);  //
      interface_display_page(26,76,0xFF00); //
      
      wrefresh(win);
      kinput_listen();
    } while (!kinput_should_quit());

    delwin(win);
    endwin();

    mem_dump();
    return 0;
}
