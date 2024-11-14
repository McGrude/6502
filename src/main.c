#include <errno.h>
#include <getopt.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cpu/cpu.h"
#include "mem/mem.h"
#include "peripherals/interface.h"
#include "peripherals/kinput.h"

#define MIN_COLUMNS 150
#define MIN_ROWS 50

uint8_t DEBUG = 0;
int opt;
int dump_flag = 0;
int follow_flag = 0;

typedef struct {
    unsigned short address;
    char *filename;
} LoadEntry;

void print_usage(char *prog_name) {
    fprintf(stderr, "Usage: %s [-d|--dump] [-f|--follow] -L 0x<hex address>:<filename>...\n", prog_name);
}

int main(int argc, char* argv[]) {

  // Dynamic list to store multiple -L entries
  LoadEntry *load_entries = NULL;
  size_t load_count = 0;

  // Long options for getopt_long
  struct option long_options[] = {
    {"dump", no_argument, 0, 'd'},
    {"follow", no_argument, 0, 'f'},
    {0, 0, 0, 0}
  };
  
  // Parse options
  while ((opt = getopt_long(argc, argv, "dfL:", long_options, NULL)) != -1) {
    switch (opt) {
    case 'd':
      dump_flag = 1;
      break;
    case 'f':
      follow_flag = 1;
      break;
    case 'L': {
      char *arg = optarg;
      char *colon_pos = strchr(arg, ':');
      
      if (colon_pos == NULL) {
	fprintf(stderr, "Error: Expected format -L 0x<hex address>:<filename>\n");
	free(load_entries);
	return EXIT_FAILURE;
      }
      
      // Split the argument at the colon
      *colon_pos = '\0';
      char *address_str = arg;
      char *filename = colon_pos + 1;
      
      // Check if the address uses the "0x" syntax
      if (strncmp(address_str, "0x", 2) != 0 && strncmp(address_str, "0X", 2) != 0) {
	fprintf(stderr, "Error: Address must use the 0x<hex> format (e.g., 0x0000).\n");
	free(load_entries);
	return EXIT_FAILURE;
      }
      
      // Convert the hex address to an integer with error checking
      char *endptr;
      errno = 0;
      long address_long = strtol(address_str, &endptr, 16);
      
      if (errno != 0 || *endptr != '\0' || address_long < 0 || address_long > 0xFFFF) {
	fprintf(stderr, "Error: Invalid address '%s'. Must be a 16-bit unsigned hex value in 0x format.\n", address_str);
	free(load_entries);
	return EXIT_FAILURE;
      }
      
      unsigned short address = (unsigned short)address_long;
      
      // Allocate memory for a new LoadEntry
      load_entries = realloc(load_entries, (load_count + 1) * sizeof(LoadEntry));
      if (load_entries == NULL) {
	perror("Memory allocation failed");
	return EXIT_FAILURE;
      }
      
      // Store the parsed address and filename
      load_entries[load_count].address = address;
      load_entries[load_count].filename = strdup(filename);
      load_count++;

      break;
    }
    case '?':
      print_usage(argv[0]);
      free(load_entries);
      return EXIT_FAILURE;
    default:
      print_usage(argv[0]);
      free(load_entries);
      return EXIT_FAILURE;
    }
  }

  // Initialize memory to zeros
  // This also sets the reset vector
  mem_init();

  // Load each file name into memory.
  //
  // !! Note : files may overlap !!
  //
  for (size_t i = 0; i < load_count; i++) {
    load_program(load_entries[i].address,load_entries[i].filename);
  }
  
  // Free allocated memory
  for (size_t i = 0; i < load_count; i++) {
    free(load_entries[i].filename);
  }
  free(load_entries);
  
  // define rows and columns
  uint32_t rows = MIN_ROWS;
  uint32_t columns = MIN_COLUMNS;

  // request a window row,columns in size
  WINDOW* win = newwin(rows,columns, 0, 0);
  if ((win = initscr()) == NULL) {
    fprintf(stderr, "[FAILED] Error initialising ncurses.\n");
    return EXIT_FAILURE;
  }

  // check that it is at least that size
  getmaxyx(win,rows,columns);
  if ( columns < MIN_COLUMNS || rows < MIN_ROWS) {
    delwin(win);
    endwin();
    fprintf(stderr, "\n\n[FAILED] Terminal Size less than %dx%d.\n",MIN_COLUMNS,MIN_ROWS);
    return EXIT_FAILURE;
  }

  // check if term is color capable.  Will require it for interface.
  if (has_colors() == FALSE) {
    endwin();
    fprintf(stderr, "\n\n[FAILED] Your terminal does not support color\n");
    return EXIT_FAILURE;
  }
    
  start_color();
  init_pair(1, COLOR_WHITE,COLOR_BLACK);
   
  curs_set(0);
  noecho();
  wrefresh(win);
  interface_display_header(1,1);
  wrefresh(win);
  
  cpu_init();
  cpu_reset();
    
  do {
    interface_display_cpu(3,4);
    // Memory Display A - Zero Page
    interface_display_page(7,1,0x0000);
    // Memory Display B - Stack
    interface_display_page(7,76,0x0100);
    // Memory Display C - Program ( may follow )
    interface_display_page(26,1,0x8000);

    
    // Memory Display D - need to update this to specify location on command line
    ///interface_display_page(26,76,0x0400);

    interface_display_page(26,76,0xFF00); 

    wrefresh(win);
    kinput_listen();
  } while (!kinput_should_quit());
  
  delwin(win);
  endwin();
  
  mem_dump();
  return 0;
}
