#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include "cpu/cpu.h"
#include "mem/mem.h"
#include "peripherals/interface.h"
#include "peripherals/kinput.h"


#define MIN_COLUMNS 150
#define MIN_ROWS 50

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

    // define rows and columns
    uint32_t rows = MIN_ROWS;
    uint32_t columns = MIN_COLUMNS;

    // request a window row,columns in size
    WINDOW* win = newwin(rows,columns, 0, 0);
    if ((win = initscr()) == NULL) {
      fprintf(stderr, "[FAILED] Error initialising ncurses.\n");
      exit(1);
    }

    // check that it is at least that size
    getmaxyx(win,rows,columns);
    if ( columns < MIN_COLUMNS || rows < MIN_ROWS) {
      delwin(win);
      endwin();
      fprintf(stderr, "\n\n[FAILED] Terminal Size less than %dx%d.\n",MIN_COLUMNS,MIN_ROWS);
      exit(1);
    }

    // check if term is color capable.  Will require it for interface.
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
    interface_display_header(0,0);
    wrefresh(win);

    cpu_init();
    cpu_reset();
    
    do {
      interface_display_cpu(1,0);
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
