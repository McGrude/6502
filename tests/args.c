#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>

typedef struct {
    unsigned short address;
    char *filename;
} LoadEntry;

void print_usage(char *prog_name) {
    fprintf(stderr, "Usage: %s [-d|--dump] [-f|--follow] -L 0x<hex address>:<filename>...\n", prog_name);
}

int main(int argc, char *argv[]) {
    int opt;
    int dump_flag = 0;
    int follow_flag = 0;

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

    // Output parsed information
    printf("Dump flag: %s\n", dump_flag ? "ON" : "OFF");
    printf("Follow flag: %s\n", follow_flag ? "ON" : "OFF");

    for (size_t i = 0; i < load_count; i++) {
        printf("Entry %zu:\n", i + 1);
        printf("  Hex Address: 0x%04X\n", load_entries[i].address);
        printf("  Filename: %s\n", load_entries[i].filename);
    }

    // Free allocated memory
    for (size_t i = 0; i < load_count; i++) {
        free(load_entries[i].filename);
    }
    free(load_entries);

    return EXIT_SUCCESS;
}
