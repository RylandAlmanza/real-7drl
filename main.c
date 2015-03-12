#include <time.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "world.h"
#include "beings.h"
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

void get_area(World *world) {
    recreate_world(world);
    char filename[255];
    sprintf(filename, "world/%d-%d.area", area_x, area_y);
    if (access(filename, F_OK) != -1) {
        load_area(world);
    } else {
        rand() % 2 == 0 ? generate_village(world) : generate_nature(world);
        player = create_being(0, 0, PLAYER, world);
    }
}


int main() {
    srand(time(NULL));
    World world;
    area_x = 0;
    area_y = 0;
    area_width = 27;
    area_height = 12;
    interface_init();

    struct stat st = {0};
    if (stat("world", &st) == -1) {
        mkdir("world", 0700);
    }

    get_area(&world);

    while (key_pressed != 'q') {
        draw_border();
        draw_world(&world);
        interface_flush();

        key_pressed = getch();
        interface_wipe();
        world.action[player](&world, player);
        Position ppos = world.position[player];
        if (ppos.x < 0) {
            save_area(&world);
            area_x--;
            get_area(&world);
            world.position[player].x = 26;
            world.position[player].y = ppos.y;
        }
        if (ppos.y < 0) {
            save_area(&world);
            area_y--;
            get_area(&world);
            world.position[player].x = ppos.x;
            world.position[player].y = 11;
        }

        if (ppos.x > 26) {
            save_area(&world);
            area_x++;
            get_area(&world);
            world.position[player].x = 0;
            world.position[player].y = ppos.y;
        }

        if (ppos.y > 11) {
            save_area(&world);
            area_y++;
            get_area(&world);
            world.position[player].x = ppos.x;
            world.position[player].y = 0;
        }
        
        for (int i = 0; i < MAX_ENTITIES; i++) {
            if (world.mask[i] != COMPONENT_NONE) {
                if ((world.mask[i] & COMPONENT_ACTION) == COMPONENT_ACTION) {
                    if (i != player && world.type[i] != PLAYER) {
                        world.action[i](&world, i);
                    }
                }
            }
        }
    }
    interface_uninit();
    save_area(&world);
    return 0;
}
