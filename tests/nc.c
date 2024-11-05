#include <ncurses.h>

int main() {
    // Initialize ncurses
    initscr();
    start_color(); // Enable colors
    curs_set(0);   // Hide cursor

    // Define color pair: foreground (white), background (black)
    init_pair(1, COLOR_WHITE, COLOR_BLACK);

    // Print text with color pair and bold attribute (for bright white effect)
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(5, 5, "This is bright white text on a black background");
    attroff(COLOR_PAIR(1) | A_BOLD);

    // Wait for user input and then clean up
    refresh();
    getch();
    endwin();

    return 0;
}
