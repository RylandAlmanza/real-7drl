#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "world.h"
#include "interface.h"
#include "globals.h"
#include "actions.h"
#include "levelgen.h"

void draw_world(World *world) {
    int i = 0;
    int x, y;
    while (world->mask[i] != COMPONENT_NONE) {
        int x = world->position[i].x;
        int y = world->position[i].y;
        char chr = world->appearance[i].chr;
        int fg = world->appearance[i].fg;
        int bg = world->appearance[i].bg;
        int attrs = world->appearance[i].attrs;
        interface_draw(x, y, chr, fg, bg, attrs);
        i++;
    }
}

void draw_border() {
    int attrs = A_BOLD | A_REVERSE;
    int fg = BLACK;
    int bg = BLACK;
    for (int x = -1; x < 28; x++) {
        interface_draw(x, -1, ' ', fg, bg, attrs);
        interface_draw(x, 12, ' ', fg, bg, attrs);
    }
    for (int y = -1; y < 13; y++) {
        interface_draw(-1, y, ' ', fg, bg, attrs);
        interface_draw(27, y, ' ', fg, bg, attrs);
    }
}

int main() {
    srand(time(NULL));
    interface_init();
    World world;
    area_x = 0;
    area_y = 0;
    area_width = 27;
    area_height = 12;
    world_width = area_width * 2;
    world_height = area_height * 2;
    rand() % 2 == 0 ? generate_village(&world) : generate_nature(&world);
    player = create_player(0, 0, &world);
    while (key_pressed != 'q') {
        draw_border();
        draw_world(&world);
        interface_flush();

        key_pressed = getch();
        interface_wipe();
        world.action[player](&world, player);
        for (int i = 0; i < MAX_ENTITIES; i++) {
            if ((world.mask[i] & COMPONENT_ACTION) == COMPONENT_ACTION) {
                if (i != player) {
                    world.action[i](&world, i);
                }
            }
        }
    }
    interface_uninit();
    return 0;
}
