#ifndef INTERFACE_H
#define INTERFACE_H

#include <ncurses.h>

static const int BLACK = COLOR_BLACK;
static const int YELLOW = COLOR_YELLOW;
static const int GREEN = COLOR_GREEN;
static const int BLUE = COLOR_BLUE;
static const int WHITE = COLOR_WHITE;
static const int TRANSPARENT = -1;

void interface_init();
void interface_draw(int x, int y, char chr, int fg, int bg);
void interface_write(int x, int y, char string[]);
void interface_flush();
void interface_wipe();
void interface_clear_line(int y);
void interface_uninit();

#endif
