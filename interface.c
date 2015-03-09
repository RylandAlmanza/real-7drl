#include <string.h>
#include "interface.h"

#define draw_offset 1

int get_color_pair(int fg, int bg) {
    return fg + (bg * 8) + 1;
}

int initialize_all_color_pairs() {
    int fg;
    int bg;
    for (bg = 0; bg < 8; bg++) {
        for (fg = 0; fg < 8; fg++) {
            init_pair(get_color_pair(fg, bg), fg, bg);
        }
    }
}

void interface_init() {
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    start_color();
    initialize_all_color_pairs();
}

void interface_draw(int x, int y, char chr, int fg, int bg, int attrs) {
    int attribute = COLOR_PAIR(get_color_pair(fg, bg));
    attron(attrs | attribute);
    mvaddch(y + draw_offset, x + draw_offset, chr);
    attroff(attrs | attribute);
}

void interface_write(int x, int y, char string[]) {
    int attribute = COLOR_PAIR(get_color_pair(WHITE, BLACK));
    attron(attribute);
    for (int i = 0; i < strlen(string); i++) {
        mvaddch(y, x + i, string[i]);
    }
    attroff(attribute);
}

void interface_flush() {
    refresh();
}

void interface_wipe() {
    clear();
}

void interface_clear_line(int y) {
    move(y, 0);
    clrtoeol();
}

void interface_uninit() {
    curs_set(1);
    endwin();
}
